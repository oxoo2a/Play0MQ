#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <zmq.h>

//	********************************************************************************
//	Packing and unpacking
//	********************************************************************************

void pack_and_send ( void *s, char *identifier, long value ) {
	static char buffer[1024];
	sprintf(buffer,"%s %ld",identifier,value);
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
		sscanf((char *) zmq_msg_data(&m),"%s %ld",buffer,value);
		int identifier_len = strlen(buffer)+1;
		*identifier = malloc(identifier_len);
		memcpy(*identifier,buffer,identifier_len);
		printf("RECV: %s %ld\n",*identifier,*value);
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
	
	// Endpoint for source connections
	char *broker_url = "tcp://*:4224";
	void *source = zmq_socket(context,ZMQ_REP);
	zmq_bind(source,broker_url);
	printf("URL for connecting sources = %s\n",broker_url);
	
	// Endpoint for subscribers (sinks)
	char *publish_url = "tcp://*:4225";
	void *publish = zmq_socket(context,ZMQ_PUB);
	zmq_bind(publish,publish_url);
	printf("URL for connecting sinks = %s\n",publish_url);
	zmq_bind(publish,"ipc://publish.ipc");
	
	// Prepare listening on mulitple endpoints
	zmq_pollitem_t endpoints[] = {
		{ source, 0, ZMQ_POLLIN, 0 },
		// { dispatch, 0, ZMQ_POLLIN, 0 }
	};
	
	while (true) {
		char *identifier;
		long number;
		
		zmq_poll(endpoints,1,-1);
		if (endpoints[0].revents & ZMQ_POLLIN) {
			recv_and_unpack(source,&identifier,&number);
			pack_and_send(source,"ack",42);
			pack_and_send(publish,identifier,number);
			free(identifier);
		}
	}
	zmq_close(source);
	zmq_close(publish);
	zmq_ctx_destroy(context);
}

//	********************************************************************************
//	play0mq_sink
//	********************************************************************************
void play0mq_sink ( int ac, char **av ) {
	if (ac < 2) failure("(1) URL of publisher and (2) one or more filter needed");
	char *publisher = av[0];	
	void *context = zmq_ctx_new();
	void *subscribe = zmq_socket(context,ZMQ_SUB);
	zmq_connect(subscribe,publisher);
	// register all filter
	for (int f=1; f<ac; f++) {
		zmq_setsockopt(subscribe,ZMQ_SUBSCRIBE,av[f],strlen(av[f]));
	}
	while (true) {
		char *identifier;
		long value;
		recv_and_unpack(subscribe,&identifier,&value);
		// Process received data
		printf("SINK: %ld received with identifier %s\n",value,identifier);
		free(identifier);
	}
	zmq_close(subscribe);
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