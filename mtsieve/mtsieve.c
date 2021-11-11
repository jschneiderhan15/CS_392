/***
 * Name          : mtsieve.c
 * Author        : Jack Schneiderhan and Cindy Zhang
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : April 17 2021
 * Description   : Sieve of Eratosthenes utilizing multithreaded processes.
 **/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/sysinfo.h>
#include <math.h>
#include <pthread.h>

int total_count = 0;
pthread_mutex_t lock;

typedef struct arg_struct {
	int start;
	int end;
} thread_args;


bool isNumber(char num[]){
	int start;
	if(num[0] == '-'){
		start = 1;
	}
	else{
		start = 0;
	}
	while(num[start] != '\0'){
		if(!isdigit(num[start]))
			return false;
		start++;
	}
	return true;
}

bool overflowCheck(char num[]){
	long overflow = atol(num);
	if(overflow > INT_MAX || overflow < INT_MIN)
		return false;
	else
		return true;
}
bool hasTwoThrees(int num){
	int digit = 0;
	int threes = 0;
	while(num > 0){
		digit = num % 10;
		if(digit == 3)
			threes++;
		num /= 10;
	}
	if(threes >= 2)
		return true;
	else 
		return false;
}
void *sieve(void* ptr){
	thread_args* tpointer = (thread_args *) ptr;
	int start = tpointer->start;
	int end = tpointer->end;
	int numPrimes = 0;
	int endSqrt = sqrt(end);
	bool *lowPrimes = (bool*) malloc (sizeof(bool) * (endSqrt + 1));
	
	for(int i = 2; i <= endSqrt; i++){
		lowPrimes[i] = true;
	}
	
	for(int i = 2; i <= endSqrt; i++){
		if(lowPrimes[i]){
			for(int j = i*i; j <= endSqrt; j+=i){
				lowPrimes[j] = false;
			}
		}
	}
	
	int highLen = end - start + 1;
	bool *highPrimes = (bool *) malloc (sizeof (bool) * (highLen + 1));
	for(int i = 0; i < highLen; i++){
		highPrimes[i] = true;
	}
	for(int p = 2; p < endSqrt; p++){
		if(lowPrimes[p]){
			int i = ceil((double) start / p) * p - start;
			if(start <= p){
				i = i + p;
			}
			for(int j = i; j < highLen; j+=p){
				highPrimes[j] = false;
			}
		}
	}
	for(int i = 2; i < highLen; i++){
		if(highPrimes[i]){
			//printf("%i\n", i + start);
			if(hasTwoThrees(i + start)){
				//printf("%i\n", i + start);
				numPrimes++;
			}
		}
	}
	int retval;
	if((retval = pthread_mutex_lock(&lock)) != 0){
		fprintf(stderr, "Warning: Cannot lock mutex. %s.\n", strerror(retval));
	}
	total_count += numPrimes;
	if((retval = pthread_mutex_unlock(&lock)) != 0){
		fprintf(stderr, "Warning: Cannot unlock mutex. %s.\n", strerror(retval));
	}
	if(start == end && (hasTwoThrees(start))){
		total_count = 1;
	}
	free(lowPrimes);
	free(highPrimes);
	pthread_exit(NULL);
}


void display_usage(){
	printf("Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
}
int main(int argc, char *argv[]){
	int opt;
	int index = 0;
	int sNum = 0, eNum = 0, tNum = 0;
	bool sExist, eExist, tExist = false;
	char* param;
	if (argc == 1){
		display_usage();
		return EXIT_FAILURE;
	}
	while((opt = getopt(argc, argv, ":s:e:t:")) != -1){
		switch(opt){
			case 's':
				sExist = true;
				param = "s";
				sNum = atoi(optarg);
				if(!isNumber(optarg)){
					printf("Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, (int)* param);
					return EXIT_FAILURE;
				}
				if(!overflowCheck(optarg)){
					printf("Error: Integer overflow for parameter '-%c'.\n", (int)* param);
					return EXIT_FAILURE;
				}
				index = optind;
				break;
			case 'e':
				eExist = true;
				param = "e";
				eNum = atoi(optarg);
				if(!isNumber(optarg)){
					printf("Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, (int)* param);
					return EXIT_FAILURE;
				}
				if(!overflowCheck(optarg)){
					printf("Error: Integer overflow for parameter '-%c'.\n", (int)* param);
					return EXIT_FAILURE;
				}
				index = optind;
				break;
			case 't':
				tExist = true;
				param = "t";
				tNum = atoi(optarg);
				if(!isNumber(optarg)){
					printf("Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, (int)* param);
					return EXIT_FAILURE;
				}
				if(!overflowCheck(optarg)){
					printf("Error: Integer overflow for parameter '-%c'.\n", (int)* param);
					return EXIT_FAILURE;
				}
				index = optind;
				break;
			default:
				if (optopt == 'e' || optopt == 's' || optopt == 't'){
					fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
				}
				else if(isprint(optopt)){
					fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
				}
				else {
					fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);
				}
				return EXIT_FAILURE;
		}
	}
	if(argv[index] != NULL){
		printf("Error: Non-option argument '%s' supplied.\n", argv[index]);
		return EXIT_FAILURE;
	}
	if(!sExist){
		printf("Error: Required argument <starting value> is missing.\n");
		return EXIT_FAILURE;
	}
	if(sNum < 2){
		printf("Error: Starting value must be >= 2.\n");
		return EXIT_FAILURE;
	}
	if(!eExist){
		printf("Error: Required argument <ending value> is missing.\n");
		return EXIT_FAILURE;
	}
	if(eNum < 2){
		printf("Error: Ending value must be >= 2.\n");
		return EXIT_FAILURE;
	}
	if(eNum < sNum){
		printf("Error: Ending value must be >= starting value.\n");
		return EXIT_FAILURE;
	}
	if(!tExist){
		printf("Error: Required argument <num threads> is missing.\n");
		return EXIT_FAILURE;
	}
	if(tNum < 1){
		printf("Error: Number of threads cannot be less than 1.\n");
		return EXIT_FAILURE;
	}
	if(tNum > (2 * get_nprocs())){
		printf("Error: Number of threads cannot exceed twice the number of processors(%d).\n", get_nprocs());
		return EXIT_FAILURE;
	}
	printf("Finding all prime numbers between %i and %i.\n", sNum, eNum);
	
	pthread_t *threads;
	if ((threads = (pthread_t *) malloc(tNum * sizeof(pthread_t))) == NULL){
		fprintf(stderr, "Error: Cannot allocate memory for threads.\n");
		return EXIT_FAILURE;
	}
	thread_args *targs;
	if((targs = (thread_args *)malloc(tNum * sizeof(thread_args))) == NULL){
		fprintf(stderr, "Error: Cannot allocate memory for targs.\n");
		return EXIT_FAILURE;
	}
	
	int defStart = sNum;
	int defEnd = eNum;
	
	int count = eNum - sNum + 1;
	if(tNum > count){
		tNum = count;
	}
	if(tNum != 1)
		printf("%i segments:\n", tNum);
	else
		printf("%i segment:\n", tNum);
	
	int remainder = count % tNum;
	count = count / tNum - 1;
	int arr[tNum];
	for(int i = 0; i < tNum; i++){
		arr[i] = count;
	}
	for(int i = 0; i < remainder; i++){
		arr[i]++;
	}
	int segmentz[tNum*2];
	for(int i = 0; i < (tNum*2); i+=2){
		segmentz[i] = sNum;
		segmentz[i+1] = sNum + arr[i/2];
		sNum = sNum + arr[i/2] + 1;
		printf("   [%i, %i]\n", segmentz[i], segmentz[i+1]);
	}
	
	int retval;
	int counter = 0;
	for(int i = 0; i < (tNum * 2); i+=2){
		targs[counter].start = segmentz[i];
		targs[counter].end = segmentz[i+1];
		if((retval = pthread_create(&threads[counter], NULL, sieve, (void*)(&targs[counter]))) != 0){
			fprintf(stderr, "Error: Cannot create thread %d. %s.\n", i, strerror(retval));
			break;
		}
		counter++;
	}
	
	for(int i = 0; i < tNum; i++){
		if(pthread_join(threads[i], NULL) != 0){
			fprintf(stderr, "Warning: Thread %d did not join properly.\n", i + 1);
		}
	}
	
	if((retval = pthread_mutex_destroy(&lock)) != 0){
		fprintf(stderr, "Error: Cannot destroy mutex. %s.\n", strerror(retval));
		free(threads);
		free(targs);
		return EXIT_FAILURE;
	}
	
	printf("Total primes between %d and %d with two or more '3' digits: %d\n", defStart, defEnd, total_count);
	free(threads);
	free(targs);
	return EXIT_SUCCESS;
}
