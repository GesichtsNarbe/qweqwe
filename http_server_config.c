#include "http_server_config.h"

http_params_t *http_server_config(char *filename, char *delim)
{
	static struct http_params_t serv_params;

	memset(&serv_params, 0, sizeof(serv_params));

	FILE *fp = fopen(filename, "r");

	if (!fp) {
		perror("");
		return NULL;
	}

	char configline[MAXLINE];
	
	int i = 0;

	while (fgets(configline, MAXLINE, fp)) {
		
		char *tmp;
		tmp = strstr(configline, delim);
		tmp += strlen(delim);

		if (i == 0) {
			uint16_t p = atoi(tmp);
			serv_params.port = p;
			printf("%d\n", serv_params.port);
		} else if (i == 1) {
			memcpy(serv_params.def_file, tmp, strlen(tmp) - 1);             
			printf("%s\n", serv_params.def_file);                         
		} else if (i == 2) { 
			memcpy(serv_params.root, tmp, strlen(tmp) - 1);             
			printf("%s\n", serv_params.root);                         
		} else if (i == 3) {
			memcpy(serv_params.listn_ip, tmp, strlen(tmp) - 1);             
			printf("%s\n", serv_params.listn_ip);                 
		}  else if (i == 4) { 
			if(strcmp(tmp, "on") == 0) 
				serv_params.logg = 1;
			else 
				serv_params.logg = 0;
			printf("%d\n", serv_params.logg);        
		}

		++i;
	}

	fclose(fp);

	return &serv_params;
}
