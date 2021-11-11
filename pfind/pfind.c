/***
 * Name          : pfind.c
 * Author        : Jack Schneiderhan and Cindy Zhang
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : March 13 2021
 * Description   : Implements the permissions finder utility.
 **/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include "perms.h"

#define BUFSIZE 128

void print_usage(){
    printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
}

int main(int argc, char *argv[]){
    int opt, df = 0, pf = 0;
    bool dfound = false, pfound = false;
    if (argc == 1 || argc > 5){
        print_usage();
        return EXIT_FAILURE;
    }
    while((opt = getopt(argc, argv, ":d:p:h")) != -1){
        switch(opt){
        case 'd':
            dfound = true;
            df++;
            if(df > 1){
            	printf("Error: Too many flags specified.\n");
            	return EXIT_FAILURE;
            }
            break;
        case 'p':
            pfound = true;
            pf++;
            if(pf > 1){
				printf("Error: Too many flags specified.\n");
				return EXIT_FAILURE;
			}
            break;
        case 'h':
            print_usage();
            return EXIT_FAILURE;
            break;
        case '?':
            printf("Error: Unknown option '%s' received.\n", argv[1]);
            return EXIT_FAILURE;
        }
    }
    if(!dfound){
        printf("Error: Required argument -d <directory> not found.\n");
        return EXIT_FAILURE;
    }
    if(!pfound){
        printf("Error: Required argument -p <permissions string> not found.\n");
        return EXIT_FAILURE;
    }

    // ----------------------open directory time ------------------------
    if(!perm_dir(argv[2], argv[4])){
    	return EXIT_FAILURE;
    }
    else if(counterr() == 0){
    	printf("<no output>\n");
    }

	return EXIT_SUCCESS;
}
