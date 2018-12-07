#include "http_server.h"

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>

static int status = 0;

struct timeval zero;

static void sign_handler(int signal) 
{
	switch (signal) {
	case SIGINT: 
		status = 0;
		break;
	}
}

int setup_signals()
{
	struct sigaction sa;

	sa.sa_handler = sign_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1) {	
		perror("cannot handle SIGINT");
		return -1;
	}

	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("signal() failed");
		return -1;
	}
	
	return 0;
}

int http_server_listen(http_server_t *server) 
{
	int connfd, ret;

	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(server->listn_fd, &rfds);



	if (setup_signals() != 0)
		return -1;

	while(status && FD_ISSET(server->listn_fd, &rfds)) {

		pid_t pid;

		ret = select((server->listn_fd + 1), &rfds, NULL, NULL, NULL);

		if(ret < 0) {

			if (status == 0) 
				break;

			else {

				perror("select() error");
				return -1;

			}
		
		} else {

			if ((connfd = accept(server->listn_fd, (struct sockaddr*)NULL, NULL)) < 0) {
				if (errno == EINTR) {
					continue;
				}

				perror("accept() failed");
				return -1;
			}

			if ((pid = fork()) == 0) {

				if ((close(server->listn_fd)) < 0) {
					perror("close() failed");
					exit(1);
				}

				if (http_server_connection_handle(connfd, server) != 0) {
					fprintf(stderr, "http_server_respond() failed");
					exit(1);
				}

				if ((close(connfd)) < 0) {
					perror("close() failed");
					exit(1);
				}

				exit(0);

			} else if (pid < 0) {

				perror("fork() failed");
				close(connfd);
				exit(1);

			} else {

				if ((close(connfd)) < 0) {
					perror("close() failed");
					exit(1);
				}
			}
		}
	}

	printf("\n");	
	
	return 0;
}

http_server_t *http_server_init(http_params_t *params) 
{
	int listenfd, on = 1;

	static struct sockaddr_in6 servaddr6;

	static struct sockaddr_in servaddr4;

	static http_server_t server;

	if (strcmp(params->listn_ip, "all") == 0) {                             //only this case is unblocking

		servaddr6.sin6_addr = in6addr_any;

		if ((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
			perror("socket() failed");
			return NULL;
		}

		if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
			perror("setsockopt() failed");
			return NULL;
		}


		int flags = fcntl(listenfd, F_GETFL, 0);
	
		if (flags < 0) {
			perror("fcntl() error");
			return NULL;
		}

		if ((fcntl(listenfd, F_SETFL, flags | O_NONBLOCK)) < 0) {
			perror("fcntl() error");
			return NULL;
		}

		memset(&servaddr6, 0, sizeof(servaddr6));
		servaddr6.sin6_family = AF_INET6;
		servaddr6.sin6_port = htons(params->port);

		if( bind(listenfd, (struct sockaddr*)&servaddr6, sizeof(servaddr6)) < 0) {
			perror("bind() error");
			close(listenfd);
			return NULL;
		}

		if (listen(listenfd, 10) < 0) {
			perror("listen() error");
			close(listenfd);
			return NULL;
		}

		server.serv_struct = (struct sockaddr_storage*)&servaddr6;
		server.listn_fd = (uint16_t)listenfd;

	} else if (inet_pton(AF_INET, params->listn_ip, &servaddr4.sin_addr)) {   //ipv4_only

		printf("ipv4!\n");

		if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket() failed");
			return NULL;
		}

		if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
			perror("setsockopt() failed");
			return NULL;
		}

		memset(&servaddr4, 0, sizeof(servaddr4));
		servaddr4.sin_family = AF_INET;
		servaddr4.sin_port = htons(params->port);

		if( bind(listenfd, (struct sockaddr*)&servaddr4, sizeof(servaddr4)) < 0) {
			perror("bind() error");
			close(listenfd);
			return NULL;
		}

		if (listen(listenfd, 10) < 0) {
			perror("listen() error");
			close(listenfd);
			return NULL;
		}

		server.serv_struct = (struct sockaddr_storage*)&servaddr4;
		server.listn_fd = (uint16_t)listenfd;


	} else if (inet_pton(AF_INET6, params->listn_ip, &servaddr6.sin6_addr)) {     //ipv6_only

		printf("ipv6!\n");


		if ((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
			perror("socket() failed");
			return NULL;
		}

		if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
			perror("setsockopt() failed");
			return NULL;
		}

		memset(&servaddr6, 0, sizeof(servaddr6));
		servaddr6.sin6_family = AF_INET6;
		servaddr6.sin6_port = htons(params->port);

		if( bind(listenfd, (struct sockaddr*)&servaddr6, sizeof(servaddr6)) < 0) {
			perror("bind() error");
			close(listenfd);
			return NULL;
		}

		if (listen(listenfd, 10) < 0) {
			perror("listen() error");
			close(listenfd);
			return NULL;
		}

		server.serv_struct = (struct sockaddr_storage*)&servaddr6;
		server.listn_fd = (uint16_t)listenfd;

	} else {
		perror("inet_pton() failed");
		return NULL;
	}


	strcpy(server.ROOT, params->root);
	strcpy(server.default_file, params->def_file);

	status = 1;
	
	return &server;
}


int http_server_uninit(http_server_t *server) 
{
	if ((close(server->listn_fd)) < 0) {
		perror("close() failed");
		return -1;
	}

	return 0;
}
