#include <stdio.h>
#include <stdlib.h>

#include "libtwit.h"
#include "stack.h"

struct tweet **add_tweet_list(stack *libtwit_stack, struct tweet *first_node)
{
	struct resource_list *resource_list = malloc(sizeof(resource_list));
	resource_list->first_node = &first_node;

	if (libtwit_stack->current_node != NULL)
		resource_list->prev = current_node;
	else
		libtwit_stack->first_node = current_node;

	libtwit_stack->current_node = &first_node;
	
	return resource_list->first_node;
}

void destroy_tweets(struct tweet *current)
{

	while (current != NULL)
	{
		struct tweet *i = current->next;
		free(current->user);
		free(current);
		current = i;
	}
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

stack *libtwit_init()
{
	stack *libtwit_stack = malloc(sizeof(stack));

	return libtwit_stack;
}

void libtwit_cleanup(*stack libtwit_stack)
{
	destroy_resource_list(libtwit_stack->first_node);
	free(libtwit_stack);
}
