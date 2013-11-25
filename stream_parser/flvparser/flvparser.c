#include <stdio.h>
#include <math.h>
#define HEADERSIZE 9

int stream_index = 0;

int main(int argc, char **argv){
	FILE *flv = fopen("test.flv", "r");

	int keyframes[10];
	int i_keyframes = 0;
	int header_end; 

	int temp;
	int header_index = 0;
	for(header_index = 0; header_index < HEADERSIZE; header_index++){
		temp = fgetc(flv);
		stream_index++;
	}

	int frame_index = 0;
	for(frame_index = 0; frame_index < 1000; frame_index++){
		int curr_index = stream_index;
		int prevtagsize = getUI(flv, 32);

		temp = fgetc(flv);
		stream_index++;

		int filterbit = getbit(temp, 2);
		int tagtype = ubdecoder(temp, 3, 5);
		int datasize = getUI(flv, 24);
		int timestamp = getUI(flv, 24);
		int timestamp_ext = getUI(flv, 8);
		int streamid = getUI(flv, 24);

		int x = 0;

		if(tagtype == 9){	
			temp = fgetc(flv);
			stream_index++;
			x++;

			int frametype = ubdecoder(temp, 0, 4);
			int codecid = ubdecoder(temp, 4, 4);
			int avcpackettype = getUI(flv, 8);
			x++;
	
			if(frametype == 1){
				keyframes[i_keyframes] = curr_index;
				i_keyframes++;

				printf("Previous Tag Size: %d\n", prevtagsize);
				printf("Stream Index: %d\n", curr_index);
				printf("Data Size: %d\n", datasize);
				printf("Timestamp: %d\n", timestamp);
				printf("Frame Type: %d\n", frametype);
				printf("Codec ID: %d\n", codecid);
				printf("AVC Packet Type: %d\n", avcpackettype);
				printf("********************\n");
			}
		}

		while(x < datasize){
			temp = fgetc(flv);
			stream_index++;
			x++;
		}

		if(i_keyframes == 1){
			keyframes[0] = stream_index-1;
		}

		if(tagtype == 18)
			header_end = stream_index - 1;
	}

	fclose(flv);

	printf("%d\n", header_end);

	FILE *r_flv = fopen("test.flv", "r");
	FILE *w_flv = fopen("out.flv", "w");

	stream_index = 0;
	int temp_int = 0;

	//write in header file
	header_index = 0;
	for(header_index = 0; header_index < HEADERSIZE; header_index++){
		temp_int = fgetc(r_flv);
		fputc(temp_int, w_flv);
		stream_index++;
	}

	//write in script frame	
	while(stream_index <= keyframes[0]){
		temp_int = fgetc(r_flv);
		fputc(temp_int, w_flv);
		stream_index++;
	}

	//skip until 3rd key_frame
	while(stream_index < keyframes[3]){
		temp_int = fgetc(r_flv);
		stream_index++;
	}

	//write in previougtagsize 
	fputc(0, w_flv);
	fputc(0, w_flv);
	fputc(0, w_flv);
	fputc(56, w_flv);
	temp_int = fgetc(r_flv);
	temp_int = fgetc(r_flv);
	temp_int = fgetc(r_flv);
	temp_int = fgetc(r_flv);
	stream_index += 4;

	//write the rest of the file into out.flv
	while((temp_int = fgetc(r_flv)) != EOF){
		fputc(temp_int, w_flv);
		stream_index++;
	}

	fclose(r_flv);
	fclose(w_flv);
}

int uidecoder(int *loc, int bitsize){
	unsigned int value = 0;
	int len;

	if(bitsize == 8)
		len = 1;
	else if(bitsize == 16)
		len = 2;
	else if(bitsize == 24)
		len = 3;
	else if(bitsize == 32)
		len = 4;

	int i;
	for(i = 0; i < len; i++){
		int power = len - i - 1;
		double temp_raised = pow(256, power);
		int casted = (int) temp_raised;
		int curr_val = temp_raised * loc[i];
		value += (curr_val);
	}

	return value;
}

int getUI(FILE *fp, int bitsize){
	int len;
	int uiarray[4]; 
	int i;

	if(bitsize == 8)
		len = 1;
	else if(bitsize == 16)
		len = 2;
	else if(bitsize == 24)
		len = 3;
	else if(bitsize == 32)
		len = 4;

	for(i = 0; i < len; i++){
		uiarray[i] = fgetc(fp);
		stream_index++;	
	}

	return uidecoder(uiarray, bitsize);
}

int ubdecoder(int byte, int start, int size){
	unsigned int value = 0;

	int i;
	for(i = start; i < size+start; i++){
		int power = size - (i - start) - 1;
		double temp_raised = pow(2, power);
		int casted = (int) temp_raised;
		int curr_val = temp_raised * getbit(byte, i);
		value += (curr_val);
	}

	return value;
}


int getbit(int byte, int loc){
	loc = 7-loc;
	return ((byte >> loc) & 1);
}
