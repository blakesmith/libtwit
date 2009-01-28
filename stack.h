#include <stdio.h>
#include <stdlib.h>

struct resource_list
{
	struct tweet **first_node;
	struct resource_list *next;
	struct resource_list *prev;
};

typedef struct
{
	struct resource_list *first_node;
	struct resource_list *current_node;

	/* Functions */

	struct tweet (*add_tweet_list)(struct tweet *first_node);
} stack;

struct tweet **add_tweet_list(struct tweet *first_node);

void destroy_tweets(struct tweet *current);

void destroy_resource_list(struct resource_list *current_resource);

