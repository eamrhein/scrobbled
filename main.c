#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512

void chomp(char *s) {
	size_t len = strlen(s);
	if (len > 0 && s[len -1] == '\n') {
		s[len -1 ] = '\0';
	}
}

int main(){
	char buffer[BUFFER_SIZE];
	FILE *fp;
	fp = popen("mpc current", "r");
	if (fp == NULL) {
		perror("Failed to run mpc");
		return 1;
	}
	if(fgets(buffer, sizeof(buffer), fp) != NULL) {
		chomp(buffer);
		if(strlen(buffer) == 0) {
			printf("No track currently playing. \n");
		} else {
			printf("Now Playing: %s\n", buffer);
		}
	} else {
		printf("No output from mpc. \n");
	}
	pclose(fp);
	return 0;
}

