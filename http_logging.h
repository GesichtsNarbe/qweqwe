#ifndef _HTTP_LOG_H_
#define _HTTP_LOG_H_

#include <stdio.h>
#include <stdlib.h>

#define LOG_FILE ""


#define ERROR "ERROR"
#define INFO "INFO"

#define VERSION "1.1"

#define LOG_ERROR(...) htttp_log_print(ERROR)
#define LOG_INFO(...) http_log_print(INFO)

int http_log_init();

int http_log_uninit();

void http_log_print(char *status, char *time, char *fmt, ...);

#endif
