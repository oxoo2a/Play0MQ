#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <zmq.h>

//	********************************************************************************
//	Packing and unpacking
//	********************************************************************************

void pack_and_send ( void *s, char *identifier, long value ) {
	static char buffer[1024];
	sprintf(buffer,"<%s> <%ld>",identifier,value);
	printf("SEND: %s\n",buffer);
	int len = strlen(buffer)+1;
	zmq_msg_t m;
	zmq_msg_init_size(&m,len);
	memcpy(zmq_msg_data(&m),buffer,len);
	zmq_msg_send(&m,s,0);
	zmq_msg_close(&m);
}

void recv_and_unpack ( void *s, char **identifier, long *value ) {
	zmq_msg_t m;
	zmq_msg_init(&m);
	int len = zmq_msg_recv(&m,s,0);
	if (len == -1) {
		*identifier = NULL;
		*value = 0;
	}
	else {
		static char buffer[1024];
		printf("RECV: %s\n",(char *) zmq_msg_data(&m));
		sscanf((char *) zmq_msg_data(&m),"<%s> <%ld>",buffer,value);
		int identifier_len = strlen(buffer)+1;
		*identifier = malloc(identifier_len);
		memcpy(*identifier,buffer,identifier_len);
	}
	zmq_msg_close(&m);
}

void failure ( char *comment ) {
	fprintf(stderr,"%s. Exiting ...",comment);
	exit(-1);
}

//	********************************************************************************
//	play0mq_source
//	********************************************************************************
void play0mq_source ( int ac, char **av ) {
	if (ac != 1) failure("URL of broker needed");
	char *broker_url = av[0];
	void *context = zmq_ctx_new();
	void *broker = zmq_socket(context,ZMQ_REQ);
	zmq_connect(broker,broker_url);
	long number = 1;
	while (true) {
		pack_and_send(broker,"number",number);
		char *identifier;
		long value;
		recv_and_unpack(broker, &identifier, &value);
		number++;
	}
	zmq_ctx_destroy(context);
}

//	********************************************************************************
//	play0mq_broker
//	********************************************************************************
void play0mq_broker ( int ac, char **av ) {
	void *context = zmq_ctx_new();
	void *source = zmq_socket(context,ZMQ_REP);
	char *broker_url = "tcp://*:4224";
	zmq_bind(source,broker_url);
	printf("broker url = %s\n",broker_url);
	while (true) {
		char *identifier;
		long number;
		recv_and_unpack(source,&identifier,&number);
		// Handle received data
		pack_and_send(source,"ack",42);
	}
	zmq_close(source);
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