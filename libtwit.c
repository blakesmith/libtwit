#include <stdio.h>
#include <curl/curl.h>
#include <libxml/parser.h>

#include "libtwit.h"

#define XML_FILE "blakesmith.xml"

xmlDocPtr open_user_timeline()
{
	xmlDocPtr doc;
	doc = xmlReadFile(XML_FILE, NULL, XML_PARSE_NOBLANKS);	
	return doc;
}

struct tweet *create_tweet(struct tweet *previous_node)
{
	struct tweet *newTweet;
	newTweet = malloc(sizeof(struct tweet));

	newTweet->prev = previous_node;
	return newTweet;
}

void destroy_tweets(struct tweet *starting_tweet)
{
	struct tweet *i;

	for (i = starting_tweet; i->next != NULL; i++)
		free(i);
}

char *get_node_value(xmlNodePtr parent, char *search_string)
{
	char *value;

	for (parent = parent->children; parent != NULL; parent = parent->next)
	{
		if ((!xmlStrcmp(parent->name, (const xmlChar *) search_string)))
		{
			value = (char *) xmlNodeGetContent(parent);
			return value;
		}
	}
}

int sanitize_string_bool(xmlChar *test_string)
{
	if ((!xmlStrcmp(test_string, (const xmlChar *) "false")))
		return 0;

	else if ((!xmlStrcmp(test_string, (const xmlChar *) "true")))
		return 1;

	else
		return -1;
}

struct tweet *parse_user_timeline(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlNodePtr children;
	struct tweet *starting_tweet;
	struct tweet *current_tweet = NULL;
	struct tweet *previous_tweet = NULL;

	for (cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status")))
		{
			previous_tweet = current_tweet; /* First loop this is NULL */
			current_tweet = create_tweet(previous_tweet);

			if (previous_tweet != NULL)
				previous_tweet->next = current_tweet;
			else
				starting_tweet = current_tweet;

//			current_tweet->user = populate_user_data()
			current_tweet->prev = previous_tweet;
			current_tweet->created_at = get_node_value(cur, "created_at");
			current_tweet->id = atoi(get_node_value(cur, "id"));
			current_tweet->text = get_node_value(cur, "text");
			current_tweet->source = get_node_value(cur, "source");
			current_tweet->truncated = sanitize_string_bool(get_node_value(cur, "truncated"));
			current_tweet->in_reply_to_status_id = atoi(get_node_value(cur, "in_reply_to_status_id"));
			current_tweet->in_reply_to_user_id = atoi(get_node_value(cur, "in_reply_to_user_id"));
			current_tweet->favorited = sanitize_string_bool(get_node_value(cur, "favorited"));
			current_tweet->in_reply_to_screen_name = get_node_value(cur, "in_reply_to_screen_name");
		}
	}
	return starting_tweet;
}

void display_tweets(struct tweet *starting_tweet)
{
	struct tweet *i;

	for (i = starting_tweet; i->next != NULL; i = i->next)
	{
		printf("created_at: %s\n"
			"id: %i\n"
			"text: %s\n"
			"source: %s\n"
			"truncated: %i\n"
			"in_reply_to_status_id: %i\n"
			"in_reply_to_user_id: %i\n"
			"favorited: %i\n"
			"in_reply_to_screen_name: %s\n", 
			
			i->created_at,
			i->id,
			i->text,
			i->source,
			i->truncated,
			i->in_reply_to_status_id,
			i->in_reply_to_user_id,
			i->favorited,
			i->in_reply_to_screen_name);
		printf("===================================\n");
	}
}


int is_authenticated()
{
	return 1;	
}


int twitter_login(CURL *curl_handle, char username, char password)
{
	if (!is_authenticated())
	{
		curl_easy_setopt(curl_handle, CURLOPT_USERNAME, username);
		curl_easy_setopt(curl_handle, CURLOPT_USERPWD, password);

		return 0;
	}
	else
		return -1;
}
		
int retrieve_xml_file()
{
	CURL *easyhandle;
	CURLcode success;

	FILE *fp;
	fp = fopen("output.html", "w");
	if (fp == NULL)
	{
		printf("Couldn't open a file for writing, exiting...");
		exit(0);
	}

	easyhandle = curl_easy_init();

	if (easyhandle)
	{
		curl_easy_setopt(easyhandle, CURLOPT_URL, "http://djblithe.com");
		curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, fp);
		success = curl_easy_perform(easyhandle);

		curl_easy_cleanup(easyhandle);

		return 0;
	}
	else
		return -1;
}

int main(int argc, char *argv[])
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	struct tweet *starting_tweet;

	doc = open_user_timeline();
	cur = xmlDocGetRootElement(doc);
	starting_tweet = parse_user_timeline(doc, cur);
	display_tweets(starting_tweet);
	destroy_tweets(starting_tweet);
	xmlFreeDoc(doc);

	return 0;
}