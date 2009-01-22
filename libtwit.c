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


void parse_user_timeline(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *value;
	xmlNodePtr children;
	struct tweet *current_tweet = NULL;
	struct tweet *previous_tweet = NULL;

	for (cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status")))
		{
			previous_tweet = current_tweet; /* First loop this is NULL */
			current_tweet = create_tweet(previous_tweet);

			for (children = cur->children; children != NULL; children = children->next)
			{
				value = xmlNodeGetContent(children);
			//	if ((!xmlStrcmp(children->name, (const xmlChar *)"user")))
//					printf("User operations go here\n");
				if ((!xmlStrcmp(children->name, (const xmlChar *)"created_at")))
					current_tweet->created_at = (char *) value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"id")))
					current_tweet->id = (const int)value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"text")))
					current_tweet->text = value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"source")))
					current_tweet->source = value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"truncated")))
					current_tweet->truncated = (const int)value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"in_reply_to_status_id")))
					current_tweet->in_reply_to_status_id = (const int)value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"in_reply_to_user_id")))
					current_tweet->in_reply_to_user_id = (const int)value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"favorited")))
					current_tweet->favorited = (const int)value;
				if ((!xmlStrcmp(children->name, (const xmlChar *)"in_reply_to_screen_name")))
					current_tweet->in_reply_to_screen_name = value;
//					printf("%s: %s\n", children->name, value);
				xmlFree(value);
			}
			printf("%s\n", value);
			printf("%s\n", current_tweet->created_at);
		}
	}
}


/*CURL *(setup_curl_handle)
{
	CURL *easyhandle;
	CURLcode success;

	curl_handle = curl_easy_init();
	return curl_handle;
}

bool is_authenticated()
{
	return True;	
}


int twitter_login(char username, char password)
{
	if (!is_authenticated)
	{
		curl_easy_setopt(curl_handle, CURLOPT_USERNAME, username);
		curl_easy_setopt(curl_handle, CURLOPT_USERPASS, password);

		return 0;
	}
	else
		return -1;
}
		


void retrieve_xml_file()
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
*/

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
