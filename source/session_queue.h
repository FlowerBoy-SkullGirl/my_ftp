#include "networking.h"

#ifndef FTP_QUEUE_H
#define FTP_QUEUE_H

#define MAX_ID 255
#define ROOT_ID 0
#define USED_ID 256
#define EMPTY_SOCKET (-1)

struct queue{
	char session_id;
	int socket;
	struct queue* next;
};

int *build_id_arr(int max_size)
{
	int *available_ids = (int *) malloc(sizeof(int) * MAX_ID);

	if (available_ids == NULL){
		fprintf(stdout, "Unable to build queue id list\n");
		exit(FTP_FALSE);
	}
	
	int j = 0;
	for (int i = 1; i <= MAX_ID; i++){
		j = i - 1;
		*(available_ids + j) = i;
	}
	return available_ids;
}

int get_available_id(int *id_list)
{
	int id = 0;
	for (int i = 0; id == 0 && i < MAX_ID; i++){
		if (*(id_list + i) == USED_ID){
			continue;
		}else{
			id = *(id_list + i);
			*(id_list + i) = USED_ID;
		}
	}

	if (id == 0)
		return FTP_FALSE;

	return id;
}

int *return_id(int *id_list, int id)
{
	int id_index = id - 1;
	*(id_list + id_index) = id;
	return id_list;
}

struct queue *queue_root()
{
	struct queue *qp = (struct queue *) malloc (sizeof(struct queue));
	qp->session_id = ROOT_ID;
	qp->socket = EMPTY_SOCKET;
	qp->next = NULL;
	return qp;
}

struct queue *queue_tail(int sock, char id)
{	
	if (id == FTP_FALSE){
		fprintf(stderr, "No session id available\n");
		exit(FTP_FALSE);
	}
	
	struct queue *qp = (struct queue *) malloc (sizeof(struct queue));
	qp->session_id = id;
	qp->socket = sock;
	qp->next = NULL;
}

struct queue *append_queue(struct queue *root, int *id_list, int sock)
{
	if (root->next == NULL){
		root->next = queue_tail(sock, get_available_id(id_list));
		return root->next;
	}

	return append_queue(root->next, id_list, sock);
}

struct queue *remove_queue(struct queue *root, int *id_list, int sock)
{
	struct queue *prev = root;
	struct queue *current = root->next;
	struct queue *next = root->next->next;

	if (root == NULL)
		return root;
	if (root->socket == sock){
		return_id(id_list, root->session_id);
		free(root);
		return current;
	}
	//Socket is not present in queue
	if (current == NULL)
		return root;

	while (current->socket != sock){
		prev = prev->next;
		current = current->next;
		next = next->next;
	}
	if (current->socket = sock){
		prev->next = next;
		return_id(id_list, current->session_id);
		free(current);
	}
	return root;
}

#endif
