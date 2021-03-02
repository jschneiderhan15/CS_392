/*******************************************************************************
 * Name        : sort.c
 * Author      : Jack Schneiderhan and Cindy Zhang
 * Date        : February 27 2021
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System. 
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum {
    STRING,
    INT,
    DOUBLE
} elem_t;

int CountRows = 0;

/**
 * Reads data from filename into an already allocated 2D array of chars.
 * Exits the entire program if the file cannot be opened.
 */
size_t read_data(char *filename, char **data) {
    // Open the file.
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open '%s'. %s.\n", filename,
                strerror(errno));
        free(data);
        exit(EXIT_FAILURE);
    }

    // Read in the data.
    size_t index = 0;
    char str[MAX_STRLEN + 2];
    char *eoln;
    while (fgets(str, MAX_STRLEN + 2, fp) != NULL) {
        eoln = strchr(str, '\n');
        if (eoln == NULL) {
            str[MAX_STRLEN] = '\0';
        } else {
            *eoln = '\0';
        }
        // Ignore blank lines.
        if (strlen(str) != 0) {
            data[index] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
            strcpy(data[index++], str);
            CountRows++;
        }
    }

    // Close the file before returning from the function.
    fclose(fp);
    return index;
}

void print_usage(){
	printf("Usage: ./sort [-i|-d] filename\n   -i: Specifies the file contains ints.\n   -d: Specifies the file contains doubles.\n   filename: The file to sort.\n   No flags defaults to sorting strings.\n");
}

/**
 * Basic structure of sort.c:
 * Parses args with getopt.
 * Opens input file for reading.
 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.
 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.
 * Closes the file, after reading in all the lines.
 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.
 * Frees all data.
 * Ensures there are no memory leaks with valgrind. 
 */
int main(int argc, char **argv) {
    int opt;
    char type = 's';
    int flags = 0;
    FILE *infile;
    if(argc != 2 && argc != 3){
        print_usage();
        return EXIT_FAILURE;
    }
    while ((opt = getopt(argc, argv, ":id")) != -1){
        switch(opt){
        case 'i':
        	flags++;
            //printf("I has been selected\n");
            if (argc == 2){
                printf("Error: No input file specified.\n");
                return EXIT_FAILURE;
            }
            else if(!(infile = fopen(argv[2], "r"))){
                printf("Error: Cannot open '%s'. ", argv[2]);
                printf(strerror(errno), "");
                printf(".\n");
                return EXIT_FAILURE;
            }
            if(flags > 1){
				printf("Error: Too many flags specified.\n");
				fclose(infile);
				return EXIT_FAILURE;
            }
            type = 'i';
            fclose(infile);
            break;
        case 'd':
        	flags++;
            //printf("D has been selected\n");
            if (argc == 2){
                printf("Error: No input file specified.\n");
                return EXIT_FAILURE;
            }
            else if(!(infile = fopen(argv[2], "r"))){
                printf("Error: Cannot open '%s'. ", argv[2]);
                printf(strerror(errno), "");
                printf(".\n");
                return EXIT_FAILURE;
            }
            type = 'd';
            if(flags > 1){
                printf("Error: Too many flags specified.\n");
                fclose(infile);
                return EXIT_FAILURE;
            }
            fclose(infile);
            break;
        case '?':
            flags++;
            if(flags != 1){
                //printf("line 132");
                printf("Error: Unknown option '%c%c' received.\n", argv[1][0], argv[1][flags]);
                print_usage();
                return EXIT_FAILURE;
            }
            printf("Error: Unknown option '%s' received.\n", argv[1]);
            print_usage();
            return EXIT_FAILURE;
        }
    }
    
	if(type == 's'){
		if(argc > 2){
			printf("Error: Too many files specified.\n");
			return EXIT_FAILURE;
		}
		if(!(infile = fopen(argv[1], "r"))){
			printf("Error: Cannot open '%s'. ", argv[1]);
			printf(strerror(errno), "");
			printf(".\n");
			return EXIT_FAILURE;
		}
		fclose(infile);
	}
	char **data = (char **)malloc(MAX_ELEMENTS);
	if(type == 'i' || type == 'd'){
		read_data(argv[2], data);
		int integers[CountRows];
		double dubbz[CountRows];
		// --------------int-----------------
		if(type == 'i'){
			for(int i = 0; i < CountRows; i++){
				integers[i] = atoi(data[i]);
			}
			quicksort(integers, CountRows, sizeof(int), int_cmp);
			for(int i = 0; i < CountRows; i++){
				printf("%d\n", integers[i]);
			}
		}
		// -------------double ----------------
		else{
			for(int i = 0; i < CountRows; i++){
				dubbz[i] = atof(data[i]);
			}
			quicksort(dubbz, CountRows, sizeof(double), dbl_cmp);
			for(int i = 0; i < CountRows; i++){
				printf("%f\n", dubbz[i]);
			}
		}
	}
	//--------------------string-----------------
	else{
		read_data(argv[1], data);
		quicksort(data, CountRows, sizeof(char*), str_cmp);
		for(int i = 0; i < CountRows; i++){
			printf("%s\n", data[i]);
		}
	}
	

	for(int i = 0; i < CountRows; i++){
		free(data[i]);
	}
	free(data);
	
    return EXIT_SUCCESS;
}
