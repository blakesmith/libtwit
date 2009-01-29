#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct tweet **add_tweet_list(struct tweet *first_node)
{
	extern struct stack *libtwit_stack;

	struct resource_list *resource_list = malloc(sizeof(resource_list));
	resource_list->first_node = &first_node;

	if (libtwit_stack->current_node != NULL)
		resource_list->prev = libtwit_stack->current_node;
	else
		libtwit_stack->first_node = libtwit_stack->current_node;

	libtwit_stack->current_node = resource_list;
	
	return resource_list->first_node;
}

void destroy_resource_list(struct resource_list *current_resource)
{

	while (current_resource != NULL)
	{
		struct resource_list *resource_i = current_resource->next;
		destroy_tweets(*(current_resource->first_node));
		free(current_resource);
		current_resource = resource_i;
	}
}

extern struct stack *libtwit_init()
{
	libtwit_stack = malloc(sizeof(struct stack));
}

void libtwit_cleanup()
{
	destroy_resource_list(libtwit_stack->first_node);
	free(libtwit_stack);
}
