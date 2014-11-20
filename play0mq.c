#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main ( int ac, char **av ) {
	if (ac < 2) {
		fprintf(stderr,"%s <role=source|broker|sink>\n",av[0]);
		exit(-1);
	}

	int major, minor, patch;
	zmq_version (&major, &minor, &patch);
	printf ("Current 0MQ version is %d.%d.%d\n", major, minor, patch);

	void *context = zmq_ctx_new();

	zmq_ctx_destroy(context);

	exit(0);
}