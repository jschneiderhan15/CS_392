/*******************************************************************************
 * Name          : chatclient.c
 * Author        : Jack Schneiderhan and Cindy Zhang
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : May 5 2021
 * Description   : Creates a chat client using TCP/IP sockets.
 ******************************************************************************/

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

int client_socket = -1;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];

int handle_stdin() {
	int messageCheck = 0;
	messageCheck = get_string(outbuf, MAX_MSG_LEN + 1);
	if(messageCheck == TOO_LONG){
		printf("Sorry, limit your message to %d characters.\n", MAX_MSG_LEN);
	}
	if(send(client_socket, outbuf, strlen(outbuf), 0) < 0){
		fprintf(stderr, "Error: Failed to send message to server. %s.\n", strerror(errno));
	}
	if(strcmp(outbuf, "bye") == 0){
		printf("Goodbye.\n");
		return 1;
	}
	//printf("%s\n", outbuf);
	return 0;
}
/*
void broadcast_buffer(int skip_index, char *buf){
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		if (i != skip_index && client_sockets[i] != -1) {
			if (send(client_sockets[i], buf, strlen(buf), 0) == -1) {
				printf("%d\n", client_sockets[i]);
				print_date_time_header(stderr);
				fprintf(stderr, "Warning: Failed to broadcast message. %s.\n",
						strerror(errno));
			}
		}
	}
}
*/

int handle_client_socket() {
	int bytes_recvd;
	if((bytes_recvd = recv(client_socket, inbuf, BUFLEN - 1, 0)) < 0 && errno != EINTR){
		fprintf(stderr, "Warning: Failed to receive incoming message. %s.\n", strerror(errno));
		return 0;
	}
	if(bytes_recvd == 0 && errno != EINTR){
		fprintf(stderr, "\nConnection to server has been lost.\n");
		return 1;
	}
	inbuf[bytes_recvd] = '\0';
	if(strcmp(inbuf, "bye") == 0){
		printf("\nServer initiated shutdown.\n");
		return 1;
	}
	printf("\n%s\n", inbuf);
	return 0;
}

int main(int argc, char *argv[]) {
	int port_num = 0, retval = EXIT_SUCCESS;
	struct sockaddr_in serv_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if(argc != 3){
		fprintf(stderr, "Usage: %s <server ip> <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	memset(&serv_addr, 0, addrlen);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	int ip_conversion = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	if(ip_conversion == 0){
		fprintf(stderr, "Error: Invalid IP address '%s'.\n", argv[1]);
		return EXIT_FAILURE;
	}

	if(!parse_int(argv[2], &port_num, "port number")){
		return EXIT_FAILURE;
	}
	if(port_num < 1024 || port_num > 65535){
		fprintf(stderr, "Error: Port must be in range [1024, 65535].\n");
		return EXIT_FAILURE;
	}

	serv_addr.sin_port = htons(port_num);

	int usernameCheck = 0;

	printf("Enter your username: ");
	fflush(stdout);

	while((usernameCheck = get_string(username, MAX_NAME_LEN + 1)) != OK){
		if(usernameCheck == TOO_LONG){
			printf("Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
			printf("Please enter your username: ");
			fflush(stdout);
		}
	}

	printf("Hello, %s. Let's try to connect to the server.\n", username);

	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}

	if(connect(client_socket, (struct sockaddr *)&serv_addr, addrlen) < 0) {
		fprintf(stderr, "Error: Failed to connect to server. %s\n", strerror(errno));
		retval = EXIT_FAILURE;
		goto EXIT;
	}

	int bytes_recvd = 0;

	if((bytes_recvd = recv(client_socket, inbuf, BUFLEN - 1, 0)) < 0){
		fprintf(stderr, "Error: Failed to receive message from server. %s.\n", strerror(errno));
		retval = EXIT_FAILURE;
		goto EXIT;
	}

	if(bytes_recvd == 0){
		fprintf(stderr, "Error: All connections are busy. Try again later.\n");
		retval = EXIT_FAILURE;
		goto EXIT;
	}

	inbuf[bytes_recvd] = 0;
	printf("\n%s\n\n", inbuf);

	if(send(client_socket, username, strlen(username), 0) < 0){
		fprintf(stderr, "Error: Failed to send message to server. %s.\n", strerror(errno));
		retval = EXIT_FAILURE;
		goto EXIT;
	}

	fd_set sockset;
	printf("[%s]:", username);
	fflush(stdout);
	while(true){
		FD_ZERO(&sockset);
		FD_SET(client_socket, &sockset);
		FD_SET(STDIN_FILENO, &sockset);
		if(select(client_socket + 1, &sockset, NULL, NULL, NULL) < 0 && errno != EINTR){
			fprintf(stderr, "Error: select() failed. %s.\n", strerror(errno));
			retval = EXIT_FAILURE;
			goto EXIT;
		}
		if(client_socket > -1 && FD_ISSET(STDIN_FILENO, &sockset)){
			if(handle_stdin() == 1){
				retval = EXIT_SUCCESS;
				goto EXIT;
			}
		}
		if(client_socket > -1 && FD_ISSET(client_socket, &sockset)){
			if(handle_client_socket() == 1){
				retval = EXIT_FAILURE;
				goto EXIT;
			}
		}
		printf("[%s]:", username);
		fflush(stdout);
	}
EXIT:
	if(fcntl(client_socket, F_GETFD) >= 0){
		close(client_socket);
	}
	return retval;
}
