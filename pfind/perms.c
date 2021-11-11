/*******************************************************************************
 * Name        : perms.c
 * Author      : Jack Schneiderhan and Cindy Zhang
 * Date        : March 14th 2021
 * Description : Program that finds files with a specified set of permissions.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include "perms.h"

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};
int counter = 0;
bool verifyp (char* perms){
	if(strlen(perms) != 9){
		return false;
	}
	for (int i = 0; i < 9; i += 3) {
		 if (perms[i] != 'r' && perms[i] != '-') {
			 return false;
		 }
		 if (perms[i+1] != 'w' && perms[i+1] != '-') {
			 return false;
		 }
		 if (perms[i+2] != 'x' && perms[i+2] != '-') {
			 return false;
		 }
	}
	return true;
}

char* perm_file(struct stat *statbuf) {
	char* buf = malloc(sizeof(char) * 10);
	int permission_valid, counter = 0;
	for (int i = 0; i < 9; i += 3) {
		 permission_valid = statbuf->st_mode & perms[i];
		 if (permission_valid) {
			 buf[counter] = 'r';
		 } else {
			 buf[counter] = '-';
		 }
		 counter++;
		 permission_valid = statbuf->st_mode & perms[i+1];
		 if (permission_valid) {
			 buf[counter] = 'w';
		 } else {
			 buf[counter] = '-';
		 }
		 counter++;
		 permission_valid = statbuf->st_mode & perms[i+2];
		 if (permission_valid) {
			 buf[counter] = 'x';
		 } else {
			 buf[counter] = '-';
		 }
		 counter++;
	}
	buf[counter] = '\0';
	return buf;
}

bool perm_dir(char* directory, char* perms) {
    char buf[PATH_MAX];
    if (realpath(directory, buf) == NULL){
    	fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", directory, strerror(errno));
    	return false;
    }
    
    if(!verifyp(perms)){
    	fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perms);
    	return false;
    }
	DIR *dir = opendir(buf);
    if(dir == NULL){
    	 fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", buf, strerror(errno));
    	 return false;
    }

    struct dirent *entry;
    struct stat sb;
    char full_filename[PATH_MAX+1];
    size_t pathlen = 0;
    
    full_filename[0] = '\0';
    if(strcmp(buf, "/")){
    	strncpy(full_filename, buf, PATH_MAX);
    }
    pathlen = strlen(full_filename) +1;
    full_filename[pathlen - 1] = '/';
    full_filename[pathlen] = '\0';
    
    while ((entry = readdir(dir)) != NULL) {
    	if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    		continue;
    	strncpy(full_filename + pathlen, entry->d_name, PATH_MAX - pathlen);
    	if (lstat(full_filename, &sb) < 0){
    		fprintf(stderr, "Error: Cannot stat file '%s'. %s\n", full_filename, strerror(errno));
    		continue;
    	}
    	
    	struct stat buf2; 
    	if(lstat(full_filename, &buf2) < 0){
    		return false;
    	}
    	char *permis = perm_file(&buf2);
    	
    	if(entry->d_type == DT_DIR){
    		if(strcmp(permis, perms) == 0){
    		    printf("%s\n", full_filename);
    		    counter++;
    		}
    		perm_dir(full_filename, perms);
    	}
    	else{
    		//printf("%s\n", full_filename);
    		if(strcmp(permis, perms) == 0){
    			printf("%s\n", full_filename);
    			counter++;
    		}
    	}
        free(permis);
    }
    closedir(dir);

    return true;
}

int counterr(){
	return counter;
}
