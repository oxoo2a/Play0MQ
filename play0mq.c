#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <zmq.h>

//	********************************************************************************
//	play0mq_source
//	********************************************************************************
void play0mq_source ( int ac, char **av ) {
	void *context = zmq_ctx_new();
	for (int i=0; i<ac; i++)
		printf("%d = %s\n",i,av[i]);
	zmq_ctx_destroy(context);
}

//	********************************************************************************
//	play0mq_broker
//	********************************************************************************
void play0mq_broker ( int ac, char **av ) {
	void *context = zmq_ctx_new();
	for (int i=0; i<ac; i++)
		printf("%d = %s\n",i,av[i]);
	zmq_ctx_destroy(context);
}

//	********************************************************************************
//	play0mq_sink
//	********************************************************************************
void play0mq_sink ( int ac, char **av ) {
	void *context = zmq_ctx_new();
	for (int i=0; i<ac; i++)
		printf("%d = %s\n",i,av[i]);
	zmq_ctx_destroy(context);
}

//	********************************************************************************
//	main and some helper functions
//	********************************************************************************
void usage (char *comment) {
	fprintf(stderr,"usage: play0mq <role=source|broker|sink>\n");
	if (comment != NULL)
		fprintf(stderr,"       %s\n",comment);
	exit(-1);
}

bool isRole ( char *role, char *value ) {
	return strcmp(role,value) == 0;
}

int main ( int ac, char **av ) {
	int major, minor, patch;
	zmq_version (&major, &minor, &patch);
	printf ("Current 0MQ version is %d.%d.%d\n", major, minor, patch);

	if (ac < 2) usage("At least a role must be specified");
	
	if (isRole("source",av[1]))
		play0mq_source(ac-2, &av[2]);
	else if (isRole("broker",av[1]))
		play0mq_broker(ac-2, &av[2]);
	else if (isRole("sink",av[1]))
		play0mq_sink(ac-2, &av[2]);
	else
		usage("Unkown role");

	exit(0);
}