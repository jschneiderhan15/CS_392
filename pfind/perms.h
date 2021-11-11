/*******************************************************************************
 * Name        : pfind.h
 * Author      : Jack Schneiderhan and Cindy Zhang
 * Date        : March 14th 2021
 * Description : Permission File.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#ifndef perms_h_
#define perms_h_

bool verifyp (char *perms);
char* perm_file(struct stat *statbuf);
bool perm_dir(char* directory, char* perms);
bool Is_dir(char* dir);
bool Is_reg(char* reg);
int counterr();

#endif
