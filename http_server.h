#ifndef _HTTP_SERV_H
#define _HTTP_SERV_H

#include "http_server_config.h"

#include <stdio.h>
#include <stdint.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1025

typedef struct http_server_t 
{
	uint16_t listn_fd;
	struct sockaddr_storage *serv_struct;
	char ROOT[PATH_MAX]; 
	char default_file[PATH_MAX];
} http_server_t;

http_server_t *http_server_init(http_params_t *);

int http_server_listen (http_server_t *); 

int http_server_connection_handle(uint16_t, http_server_t *);

int http_server_uninit(http_server_t*);



typedef struct http_request_t                                                   
{                                                                               
         char method[MAXLINE];                                                   
         char URL[MAXLINE];                                                      
         char version[MAXLINE];                                                  
} http_request_t;   





int http_recive_request(char *mesg, uint16_t connfd);

int http_error_handle(int status_code, int connfd);

int http_connection_handle(uint16_t fd, http_server_t *server);

int http_request_handle(char *mesg, int connfd, http_server_t *server);

int http_request_parse(char *mesg, http_request_t *request);

int http_request_validate(http_request_t *request);

int http_perform_request(http_request_t *request, http_server_t *server, int connfd);

#endif
