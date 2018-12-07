#ifndef _HTTP_CONF_H
#define _HTTP_CONF_H

#include <stdio.h>
#include <stdint.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define CONFIG "config_file"
#define DELIM "="
#define MAXLINE 1025

typedef struct http_params_t
{
	uint16_t port;
	char root[PATH_MAX];
	char def_file[PATH_MAX];
	char listn_ip[INET6_ADDRSTRLEN];
	uint8_t logg;
} http_params_t;

http_params_t *http_server_config(char *, char *);

#endif
