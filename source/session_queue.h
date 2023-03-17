#include "networking.h"

#ifndef FTP_QUEUE_H
#define FTP_QUEUE_H

#define MAX_ID 255

struct queue{
	char session_id;
	struct queue* next;
};

struct queue *queue_root(char session_value)
{
	struct queue *qp = (struct queue *) malloc (sizeof(struct queue));
	qp->session_id = session_value;
	qp->next = NULL;
	return qp;
}

struct queue *append_queue(struct queue *root)
{
	next_session_id = root->session_id + 1;
	if (next_session_id > MAX_ID)
		next_session_id //STOPPED WORKING HERE
	if (root->next == NULL){
		root->next = queue_root((root->session_id + 1));

	}
}

#endif
