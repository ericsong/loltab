#define _POSIX_SOURCE
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define DETECTION_PROGRAM "./detect_sb"
#define STREAMING_PROGRAM "playbin.py"
#define STATIC_DIR        "static/"
#define SCOREBOARD_DIR    "scoreboards/"
#define SCOREBOARD_IMG    "scoreboard.jpg"

static char exit_signal; /* doesn't matter if this has race conditions */
static char debug;
static int  onThrottle;

static void exit_handler(int signum);
static void throttle(int signum);
static int  start_streaming(char *name, char *quality);
static int  detect_scoreboard(char *source, char *dest);
static void raise_scoreboard_event(char *name);

int main(int argc, char **argv) {
  char  url[256];
  char  imagename[256];
  int   playbin_pid;
  int   i;
  DIR   *dp;
  char  new_scoreboard;
  uint64_t          count;
  struct sigaction  alarm_handler;

  /* get args */
  if (argc < 3) {
    fprintf(stderr, "\
Usage: ./StreamManager <streamer name> <medium|high|best> [-d|--debug]\n\
  example: ./StreamManager flosd best --debug\n");
    return 1;
  }
  for (i = 3; i < argc; i++)
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0)
      debug = 1;

  /* make static and scoreboard directories */
  if ((dp = opendir(STATIC_DIR)) == NULL)
    mkdir(STATIC_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  else
    closedir(dp);
  if ((dp = opendir(SCOREBOARD_DIR)) == NULL)
    mkdir(SCOREBOARD_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  else
    closedir(dp);

  /* set throttle and exit signals */
  signal(SIGINT, exit_handler);
  alarm_handler.sa_handler = throttle;
  sigaction(SIGALRM, &alarm_handler, NULL);
  if (debug)
    printf("[STREAM MANAGER] Starting stream analysis.\n");

  /* start capturing stream */
  sprintf(url, "twitch.tv/%s", argv[1]);
  if ((playbin_pid = start_streaming(url, argv[2])) == -1) {
    if (debug)
      fprintf(stderr, "[STREAM MANAGER] Error: Cannot connect to stream.\n");
    return 1;
  }

  /* start detecting scoreboards */
  for (new_scoreboard = 0, count = 0;;) {
    if (exit_signal)
      break;
    sprintf(imagename, "img%lu.jpg", count);
    if (access(imagename, F_OK) != -1) {
      new_scoreboard = detect_scoreboard(imagename, SCOREBOARD_DIR SCOREBOARD_IMG) || new_scoreboard;
      if (!debug)
        unlink(imagename);
      else
        printf("[STREAM MANAGER] Found %s's %s\n", argv[1], imagename);
      count++;
    }
    if (!onThrottle && new_scoreboard) {
      /* be sure that the newest scoreboard is always in that filename */
      raise_scoreboard_event(argv[1]);
      onThrottle = 1;
      new_scoreboard = 0;
      alarm(1);
    }
  }

  /* exit program when finished */
  kill(playbin_pid, SIGINT);
  waitpid(playbin_pid, NULL, 0);
  if (!debug) {
    if (access(STATIC_DIR SCOREBOARD_IMG, F_OK) != -1)
      unlink(STATIC_DIR SCOREBOARD_IMG);
    rmdir(STATIC_DIR);
    if (access(SCOREBOARD_DIR SCOREBOARD_IMG, F_OK) != -1)
      unlink(SCOREBOARD_DIR SCOREBOARD_IMG);
    rmdir(SCOREBOARD_DIR);
  } else {
    printf("[STREAM MANAGER] Stream analysis closed successfully\n");
  }
  return 0;
}

/* start the stream */
int start_streaming(char *name, char *quality) {
  int pid;
  char buffer[256];
  if ((pid = fork()) == 0) {
    char *args[5];
    args[0] = "python";
    args[1] = STREAMING_PROGRAM;
    args[2] = name;
    args[3] = quality;
    args[4] = NULL;
    execvp("python", args);
    if (debug)
      fprintf(stderr, "[STREAM MANAGER] Error: Cannot create playbinStream process.\n");
    exit(1);
  } else {
    if (strstr(buffer, "[Errno 2]") != NULL) {
      if (debug)
        fprintf(stderr, "[STREAM MANAGER] Error: Cannot create playbinStream process.\n");
      kill(pid, SIGINT);
      waitpid(pid, NULL, 0);
      return -1;
    }
  }
  return pid;
}

/* get detection status */
int detect_scoreboard(char *source, char *dest) {
  int pid;
  int fd[2];
  char buffer[256];

  pipe(fd);
  if ((pid = fork()) == 0) {
    char *args[4];
    args[0] = DETECTION_PROGRAM;
    args[1] = source;
    args[2] = dest;
    args[3] = NULL;
    close(1);
    dup(fd[1]);
    close(fd[0]);
    close(fd[1]);
    execvp(DETECTION_PROGRAM, args);
    if (debug)
      fprintf(stderr, "[STREAM MANAGER] Error: Cannot create detect scoreboard process.\n");
    exit(1);
  } else {
    waitpid(pid, NULL, 0);
    read(fd[0], buffer, sizeof buffer);
    close(fd[0]);
    close(fd[1]);
  }
  return strstr(buffer, "scoreboard") == NULL;
}

/* exit the program */
void exit_handler(int signum) {
  if (signum == SIGINT || signum == SIGKILL || signum == SIGSTOP) {
    if (debug)
      printf("[STREAM MANAGER] Exit signal detected.\n");
    exit_signal = 1;
  }
}

/* throttle the output */
void throttle(int signum) {
  if (signum == SIGALRM && onThrottle)
    onThrottle = 0;
}

/* raise scoreboard event */
void raise_scoreboard_event(char *name) {
  int pid;
  if (access(SCOREBOARD_DIR SCOREBOARD_IMG, F_OK) == -1) {
    if (debug)
      fprintf(stderr, "[STREAM MANAGER] Error: Cannot access the scoreboard in the directory " SCOREBOARD_DIR ".\n");
    return;
  }
  if (access(STATIC_DIR SCOREBOARD_IMG, F_OK) == -1) {
    DIR *dp;
    if ((dp = opendir(STATIC_DIR)) == NULL)
      mkdir(STATIC_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    else
      closedir(dp);
    /* TODO: this part will have to be modified */
    chdir(STATIC_DIR);
    symlink("../" SCOREBOARD_DIR SCOREBOARD_IMG, SCOREBOARD_IMG);
    chdir("..");
  }
  if ((pid = fork()) == 0) {
    char *args[4];
    args[0] = "python";
    args[1] = "raise_scoreboard_event.py";
    args[2] = name;
    args[3] = NULL;
    execvp("python", args);
    if (debug)
      fprintf(stderr, "[STREAM MANAGER] Error: Cannot raise scoreboard event.\n");
    exit(1);
  }
}
