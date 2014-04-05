#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include "tqueue.h"

/* .data */
#define detection_program	"./detect_sb"
#define extraction_program	"./extract_data"
#define streamer_program	"playbinStream.py"
#define scoreboard_dir		"scoreboards"
#define numThreads			2

/* .globl */
static int		exit_signal; /* doesn't matter if this has race conditions */
static int		end_asap;
static tqueue	*ImageQueue;
static int		debug;
static int		in_game;
static char		*ofname;

/* .proto */
static void exit_handler(int signum);
static void *detectionThread(void *args);
static void *extractionThread(void *args);

/* .text */
int main(int argc, char **argv) {
	char		url[256];
	char		*quality;
	int			i;
	DIR			*dp;
	pthread_t	threads[numThreads];
	void		*(*routines[numThreads])(void *);
	int			pid;

	/* set up args */
	if (argc < 4) {
		fprintf(stderr, "\
Invalid usage: ./StreamManager <streamer name> <medium|high|best>\n\
		<output file> [--force|-f] [--debug|-s]\n\n\
	example: ./queueManager flosd best stdout -f -s\n\
	Options:\n\
		--force, -f\tForce ingame status\n\
		--debug, -d\tDo not delete anything, print more messages\n");
		return 1;
	}
	strcpy(url, 	"twitch.tv/");
	strcat(url, 	argv[1]);
	quality = 		argv[2];
	ofname =		argv[3];
	routines[0] = 	detectionThread;
	routines[1] = 	extractionThread;

	for (i = 4; i < argc; i++) {
		if (strcmp(argv[i], "--force") == 0 || strcmp(argv[i], "-f") == 0) {
			in_game = 1;
			continue;
		}
		if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
			debug = 1;
		}
	}

	/* Create queues and their associated locks */
	ImageQueue = (tqueue *)malloc(sizeof(tqueue));
	if (tqueue_init(ImageQueue) != 0) {
		fprintf(stderr, "Not able to create Image Queue...\n");
		free(ImageQueue);
		return 1;
	}

	/* check for scoreboard directory, we check here to avoid race conditions */
	dp = opendir(scoreboard_dir);
	if (dp == NULL) {
		mkdir(scoreboard_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	} else {
		closedir(dp);
	}
	
	/* set exit signal */
	signal(SIGINT, exit_handler);
	if (debug) {
		printf("Starting program\n");
	}

	/* initialize threads */
	for (i = 0; i < numThreads; i++) {
		if (pthread_create(&threads[i], NULL, routines[i], NULL) != 0) {
			fprintf(stderr, "Not able to create thread %d...\n", i);
			return 1;
		}
	}
	
	/* start stream */
	pid = fork();
	if (pid == 0) {
		char *args[5];
		args[0] = "python";
		args[1] = streamer_program;
		args[2] = url;
		args[3] = quality;
		args[4] = 0;
		execvp("python", args);
		if (debug) {
			printf("Panic: Exiting playbinStream...\n");
		}
		_exit(0);
	} else {
		waitpid(pid, NULL, 0);
	}

	/* wait for threads to finish */
	end_asap = 1;
	for (i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);
	}

	/* cleanup */
	if (!debug) {
		rmdir(scoreboard_dir);
	}
	
	tqueue_clean(ImageQueue);
	tqueue_destroy(ImageQueue);
	free(ImageQueue);
	if (debug) {
		printf("Program closed successfully\n");
	}
	return 0;
}

void exit_handler(int signum) {
	if (signum == SIGINT || signum == SIGKILL || signum == SIGSTOP) {
		exit_signal = 1;
	}
}

void *detectionThread(void *args) {
	int currentImageIndex;
	char *image_name;
	FILE *fp;
	int fd[2];
	int pid;
	char buffer[128];

	currentImageIndex = 1;
	image_name = (char *)calloc(64, sizeof(char));
	sprintf(image_name, "image%d.jpg", currentImageIndex++);

	while (!exit_signal) {
		if (end_asap) {
			tqueue_signal(ImageQueue);
			pthread_exit(0);
		}
		/* find image */
		if ((fp = fopen(image_name, "r")) == 0) {
			continue;
		} else {
			fclose(fp);
		}

		if (debug) {
			printf("Found image\n");
		}

		/* if image found, detect for scoreboard */
		pipe(fd);
		pid = fork();
		if (pid == 0) {
			char *args[3];
			args[0] = detection_program;
			args[1] = image_name;
			args[2] = 0;
			close(1);
			dup(fd[1]);
			close(fd[0]);
			close(fd[1]);
			execvp(detection_program, args);
			fprintf(stderr, "Panic: Exec failed for detection thread\n");
			free(image_name);
			_exit(1);
		} else {
			close(fd[1]);
			waitpid(pid, NULL, 0);
			read(fd[0], buffer, sizeof(buffer));
			close(fd[0]);
			if (!debug) {
				unlink(image_name);
			}
		}

		if (strstr(buffer, "scoreboard") == 0) {
			/* scoreboard detected */
			strcpy(buffer, image_name);
			sprintf(image_name, "./%s/%s", scoreboard_dir, buffer);
		} else {
			/* move on to the next image */
			sprintf(image_name, "image%d.jpg", currentImageIndex++);
			continue;
		}

		/* if image has detected scoreboard, then place into queue */
		if (!tqueue_lock(ImageQueue)) {
			tqueue_wait(ImageQueue);
		}
		tqueue_enqueue(ImageQueue, (void *)image_name);
		tqueue_unlock(ImageQueue);
		tqueue_signal(ImageQueue);

		/* find next image */
		image_name = (char *)calloc(64, sizeof(char));
		sprintf(image_name, "image%d.jpg", currentImageIndex++);
	}
	pthread_exit(0);
}

void *extractionThread(void *args) {
	char *image_name;
	int pid;

	while (!exit_signal) {
		/* open image queue to get image */
		if (!tqueue_lock(ImageQueue)) {
			tqueue_wait(ImageQueue);
		}
		if (ImageQueue->size == 0) {
			if (end_asap) {
				tqueue_unlock(ImageQueue);
				tqueue_signal(ImageQueue);
				pthread_exit(0);
			} else {
				tqueue_wait(ImageQueue);
			}
		}
		image_name = (char *)tqueue_dequeue(ImageQueue);
		tqueue_unlock(ImageQueue);
		tqueue_signal(ImageQueue);

		if (debug) {
			printf("Extracting scoreboard\n");
		}
		
		/* extract image */
		pid = fork();
		if (pid == 0) {
			char *args[4];
			args[0] = extraction_program;
			args[1] = image_name;
			args[2] = ofname;
			args[3] = 0;
			execvp(extraction_program, args);
			fprintf(stderr, "Panic: Exec failed for extraction thread\n");
			_exit(1);
		} else {
			waitpid(pid, NULL, 0);
			if (!debug) {
				unlink(image_name);
			}
		}
	}
	pthread_exit(0);
}

