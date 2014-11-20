#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <zmq.h>

void usage (char *comment) {
	fprintf(stderr,"usage: play0mq <role=source|broker|sink>\n");
	if (comment != NULL)
		fprintf(stderr,"       %s\n",comment);
	exit(-1);
}

bool isRole ( char *role, char *value ) {
	return strcmp(role,value) == 0;
}

enum ROLE { SOURCE, BROKER, SINK };
enum ROLE role;

int main ( int ac, char **av ) {
	int major, minor, patch;
	zmq_version (&major, &minor, &patch);
	printf ("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
	
	if (ac < 2) usage("At least a role must be specified");
	
	if (isRole("source",av[1]))
		role = SOURCE;
	else if (isRole("broker",av[1]))
		role = BROKER;
	else if (isRole("sink",av[1]))
		role = SINK;
	else
		usage("Unkown role");
		
	void *context = zmq_ctx_new();

	zmq_ctx_destroy(context);

	exit(0);
}