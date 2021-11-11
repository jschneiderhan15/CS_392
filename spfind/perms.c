/***
 * Name          : perms.c
 * Author        : Jack Schneiderhan and Cindy Zhang
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : March 29, 2021
 * Description   :
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

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};

bool verifyp (char* perms){
    if(strlen(perms) != 9){
        printf("line 16");
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