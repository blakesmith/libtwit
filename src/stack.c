#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
#include "stack.h"

struct stack *libtwit_stack;
char *libtwit_twitter_username;
char *libtwit_twitter_password;

struct tweet 
*add_tweet_list(struct tweet *first_node)
{
	struct resource_list *current_resource = malloc(sizeof(struct resource_list));

	if (current_resource == NULL) {
		printf("Out of memory, dying...");
		exit(0);
	}

	current_resource->first_node = first_node;

	if (libtwit_stack->current_node != NULL) {
		current_resource->prev = libtwit_stack->current_node;
		libtwit_stack->current_node->next = current_resource;
	}
	else {
		libtwit_stack->first_node = current_resource;
		current_resource->next = NULL;
		current_resource->prev = NULL;
	}

	libtwit_stack->current_node = current_resource;
	
	return current_resource->first_node;
}

void 
destroy_resource_list(struct resource_list *current_resource)
{

	while (current_resource != NULL) {
		struct resource_list *resource_i = current_resource->next;
		destroy_tweets(current_resource->first_node);
		free(current_resource);
		current_resource = resource_i;
	}
}

void 
*libtwit_init()
{
	libtwit_stack = malloc(sizeof(struct stack));

	if (libtwit_stack == NULL) {
		printf("Out of memory, dying...");
		exit(0);
	}
}

void 
libtwit_deinit()
{
	destroy_resource_list(libtwit_stack->first_node);
	free(libtwit_stack);
	free(libtwit_twitter_username);
	free(libtwit_twitter_password);
}
