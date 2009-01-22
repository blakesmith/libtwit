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

void parse_user_timeline(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlNodePtr children;
	struct tweet *current_tweet = NULL;
	struct tweet *previous_tweet = NULL;

	for (cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status")))
		{
			previous_tweet = current_tweet; /* First loop this is NULL */
			current_tweet = create_tweet(previous_tweet);

//			current_tweet->user = populate_user_data()
			current_tweet->created_at = get_node_value(cur, "created_at");
			current_tweet->id = atoi(get_node_value(cur, "id"));
			current_tweet->text = get_node_value(cur, "text");
			current_tweet->source = get_node_value(cur, "source");
			current_tweet->truncated = sanitize_string_bool(get_node_value(cur, "truncated"));
			current_tweet->in_reply_to_status_id = atoi(get_node_value(cur, "in_reply_to_status_id"));
			current_tweet->in_reply_to_user_id = atoi(get_node_value(cur, "in_reply_to_user_id"));
			current_tweet->favorited = sanitize_string_bool(get_node_value(cur, "favorited"));
			current_tweet->in_reply_to_screen_name = get_node_value(cur, "in_reply_to_screen_name");
			printf("========================\n");
			printf("%i\n", current_tweet->id);
			printf("%i\n", current_tweet->truncated);
			printf("%i\n", current_tweet->favorited);
			printf("%s\n", current_tweet->created_at);
			printf("%s\n", current_tweet->text);
		}
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

	doc = open_user_timeline();
	cur = xmlDocGetRootElement(doc);
	parse_user_timeline(doc, cur);
	xmlFreeDoc(doc);

	return 0;
}
