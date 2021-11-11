/***
 * Name          : spfind.c
 * Author        : Jack Schneiderhan and Cindy Zhang
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : March 29, 2021
 * Description   : Using Permission Find program and sorting its output.
 **/

#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "perms.h"

void print_usage(){
    printf("Usage: ./spfind -d <directory> -p <permissions string> [-h]\n");
}

int main(int argc, char *argv[]){   
    int pfindpipe[2], sortpipe[2];
    if(pipe(pfindpipe) < 0){
    	fprintf(stderr, "Error: Cannot create pipe pfindpipe. %s.\n", strerror(errno));
    	return EXIT_FAILURE;
    }
    if (pipe(sortpipe) < 0){
    	fprintf(stderr, "Error: Cannot create pipe sortpipe. %s\n", strerror(errno));
    	return EXIT_FAILURE;
    }
    
    pid_t pid[2];
    
    if((pid[0] = fork()) == 0){
    	//checking for pfind
    	close(pfindpipe[0]);
    	if(dup2(pfindpipe[1], STDOUT_FILENO) == -1){
    		fprintf(stderr, "Error: dup2 failed. %s.\n", strerror(errno));
    		close(pfindpipe[1]);
    		close(sortpipe[0]);
    		close(sortpipe[1]);
    		exit(EXIT_FAILURE);
    	}
    	
    	close(sortpipe[0]);
    	close(sortpipe[1]);
    	
    	if(execv("pfind", argv) < 0){
    		fprintf(stderr, "Error: pfind failed. %s.\n", strerror(errno));
    		exit(EXIT_FAILURE);
    	}
    }
    if((pid[1] = fork()) == 0){
       	//checking for sort
       	close(pfindpipe[1]);
       	if(dup2(pfindpipe[0], STDIN_FILENO) == -1){
       		fprintf(stderr, "Error: dup2 failed. %s.\n", strerror(errno));
       		close(pfindpipe[0]);
       		close(sortpipe[0]);
       		close(sortpipe[1]);
       		exit(EXIT_FAILURE);
       	}
       	close(sortpipe[0]);
       	if(dup2(sortpipe[1], STDOUT_FILENO) == -1){
       		fprintf(stderr, "Error: dup2 failed. %s.\n", strerror(errno));
       		close(pfindpipe[0]);
       		close(sortpipe[1]);
       		exit(EXIT_FAILURE);
       	}
       	
       	if(execlp("sort", "sort", NULL) < 0){
       		fprintf(stderr, "Error: sort failed. %s.\n", strerror(errno));
       		exit(EXIT_FAILURE);
       	}
     }
    
    close(sortpipe[1]);
    if(dup2(sortpipe[0], STDIN_FILENO) == -1){
    	fprintf(stderr, "Error: dup2 failed. %s.\n", strerror(errno));
    	close(sortpipe[0]);
    	close(pfindpipe[0]);
    	close(pfindpipe[1]);
    	exit(EXIT_FAILURE);
    }
    close(pfindpipe[0]);
    close(pfindpipe[1]);
    int counter = 0;
    char buffer[4096];
    while(1) {
    	size_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
    	if(count == -1){
    		if(errno == EINTR){
    			continue;
    		}
    		else{
    			perror("read()");
    			exit(EXIT_FAILURE);
    		}
    	} else if(count == 0){
    		break;
    	}
    	else{

    		for(int i = 0; i < count; i++){
    			if(buffer[i] == '\n')
    				counter++;
    		}
    		write(STDOUT_FILENO, buffer, count);
    	}
    	
    }
    int status1, status2;
      pid_t first_child = wait(&status1);
      if (first_child == -1) {
          fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
      }

      pid_t second_child = wait(&status2);
      if (second_child == -1) {
          fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
      }
      //printf("status1: %i, status2: %i\n", status1, status2);
      if(!(WEXITSTATUS(status1) == EXIT_FAILURE) && !(WEXITSTATUS(status2) == EXIT_FAILURE)){
          printf("Total matches: %d\n", counter);
      }

      return !(WIFEXITED(status1) && WEXITSTATUS(status1) == EXIT_SUCCESS) || !(WIFEXITED(status1) && WEXITSTATUS(status1) == EXIT_SUCCESS);
}
