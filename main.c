#include "http_server.h"
#include "http_server_config.h"
#include "http_logging.h"

#include <signal.h>

int main(int agrc, char **argv)
{
	int ret = 0;

	http_params_t *server_params;

	if((server_params = http_server_config(CONFIG, DELIM)) == NULL) {

		fprintf(stderr, "config eror\n");

		exit(EXIT_FAILURE);
	}

	http_server_t *server = http_server_init(server_params);

	if (!server) {

		fprintf(stderr, "server failed while initializing");

		exit(EXIT_FAILURE);
	}

	if (!http_log_init(server)) {
		
		fprintf(stderr, "server failed while initializing logfile");

		exit(EXIT_FAILURE);
	}   //new

	if (!(ret = http_server_listen(server))) {
	
		if (http_server_uninit(server) < 0) 
		exit(EXIT_FAILURE);
	
	} else if (ret < 0) {
	
		fprintf(stderr, "error occurred while http_listen\n");
		exit(EXIT_FAILURE);
	
	}

	exit(EXIT_SUCCESS);
}
