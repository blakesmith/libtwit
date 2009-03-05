#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <libxml/parser.h>

#include "core.h"
#include "stack.h"

xmlDocPtr 
open_xml_file(struct xml_memory *mem)
{
	xmlDocPtr doc;
	doc = xmlReadMemory(mem->memory, mem->size, "noname", NULL, XML_PARSE_NOBLANKS);	
	return doc;
}

struct tweet 
*create_tweet(struct tweet *previous_node)
{
	struct tweet *newTweet;
	newTweet = malloc(sizeof(struct tweet));

	if (newTweet == NULL) {
		printf("Out of memory, dying...");
		exit(0);
	}

	newTweet->prev = previous_node;
	return newTweet;
}

void 
destroy_tweets(struct tweet *current)
{

	while (current != NULL) {
		struct tweet *i = current->next;
		free(current->user);
		free(current);
		current = i;
	}
}

struct twitter_user 
*create_user()
{
	struct twitter_user *newUser;
	newUser = malloc(sizeof(struct twitter_user));

	return newUser;
}

xmlChar 
*get_node_value(xmlNodePtr parent, char *search_string)
{
	xmlChar *value;

	for (parent = parent->children; parent != NULL; parent = parent->next) {
		if ((!xmlStrcmp(parent->name, (const xmlChar *) search_string))) {
			value = xmlNodeGetContent(parent);
			return value;
		}
	}
}

xmlNodePtr 
get_node_ptr(xmlNodePtr parent, char *search_string)
{
	for (parent = parent->children; parent != NULL; parent = parent->next) {
		if ((!xmlStrcmp(parent->name, (const xmlChar *) search_string))) {
			return parent;
		}
	}
}

struct twitter_user 
*get_user_data(xmlNodePtr parent)
{
	struct twitter_user *new_user = create_user();

	new_user->id = atoi(get_node_value(parent, "id"));
	new_user->name = get_node_value(parent, "name");
	new_user->screen_name = get_node_value(parent, "screen_name");
	new_user->location = get_node_value(parent, "location");
	new_user->description = get_node_value(parent, "description");
	new_user->profile_image_url = get_node_value(parent, "profile_image_url");
	new_user->url = get_node_value(parent, "url");
	new_user->prot = sanitize_string_bool(get_node_value(parent, "protected"));
	new_user->followers_count = atoi(get_node_value(parent, "followers_count"));

	return new_user;
}


int 
sanitize_string_bool(xmlChar *test_string)
{
	if ((!xmlStrcmp(test_string, (const xmlChar *) "false")))
		return 0;

	else if ((!xmlStrcmp(test_string, (const xmlChar *) "true")))
		return 1;

	else
		return -1;
}

struct tweet 
*parse_tweets(xmlNodePtr cur)
{
	xmlNodePtr children;
	struct tweet *starting_tweet;
	struct tweet *current_tweet = NULL;
	struct tweet *previous_tweet = NULL;

	if (cur == NULL) {
		printf("No tweets to parse!\n");
		exit(0);
	}

	for (cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status"))) {
			previous_tweet = current_tweet; /* First loop this is NULL */
			current_tweet = create_tweet(previous_tweet);

			if (previous_tweet != NULL)
				previous_tweet->next = current_tweet;
			else
				starting_tweet = current_tweet;

			strptime((char *)get_node_value(cur, "created_at"), "%a %b %d %H:%M:%S +0000 %Y", &(current_tweet->created_at)); /* Sets the created_at */
			current_tweet->user = get_user_data(get_node_ptr(cur, "user"));
			current_tweet->next = NULL;
			current_tweet->prev = previous_tweet;
			current_tweet->id = atoi(get_node_value(cur, "id"));
			current_tweet->text = get_node_value(cur, "text");
			current_tweet->source = get_node_value(cur, "source");
			current_tweet->truncated = sanitize_string_bool(get_node_value(cur, "truncated"));
			current_tweet->in_reply_to_status_id = atoi(get_node_value(cur, "in_reply_to_status_id"));
			current_tweet->in_reply_to_user_id = atoi(get_node_value(cur, "in_reply_to_user_id"));
			current_tweet->favorited = sanitize_string_bool(get_node_value(cur, "favorited"));
			current_tweet->in_reply_to_screen_name = get_node_value(cur, "in_reply_to_screen_name");
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"error")))
			return NULL;
	}
	add_tweet_list(starting_tweet);

	return starting_tweet;
}

void 
display_tweets(struct tweet *starting_tweet)
{
	struct tweet *i;

	for (i = starting_tweet; i != NULL; i = i->next) {
		char created_char[SLENGTH];
		strftime(created_char, SLENGTH, "%a %b %d %H:%M:%S %Y", &(i->created_at));
		printf("created_at: %s\n"
			"id: %i\n"
			"text: %s\n"
			"source: %s\n"
			"truncated: %i\n"
			"in_reply_to_status_id: %i\n"
			"in_reply_to_user_id: %i\n"
			"favorited: %i\n"
			"in_reply_to_screen_name: %s\n" 
			"user: \n"
			"\tid: %i\n"
			"\tname: %s\n"
			"\tscreen_name: %s\n"
			"\tlocation: %s\n"
			"\tdescription: %s\n"
			"\tprofile_image_url: %s\n"
			"\turl: %s\n"
			"\tprotected: %i\n"
			"\tfollowers_count: %i\n"
			"===================================\n",
			
			created_char,
			i->id,
			i->text,
			i->source,
			i->truncated,
			i->in_reply_to_status_id,
			i->in_reply_to_user_id,
			i->favorited,
			i->in_reply_to_screen_name,
			i->user->id,
			i->user->name,
			i->user->screen_name,
			i->user->location,
			i->user->description,
			i->user->profile_image_url,
			i->user->url,
			i->user->prot,
			i->user->followers_count);
	}
}


int 
is_authenticated()
{
	if (libtwit_twitter_username && libtwit_twitter_password)
		return 1;	
	else
		return 0;
}


int 
twitter_login(char *username, char *password)
{

	if (!is_authenticated()) {
		CURL *curl_handle;
		CURLcode success;

		char build_url[SLENGTH];
		strcat(build_url, ACCOUNT_URL);
		strcat(build_url, VERIFY_CREDENTIALS);

		curl_handle = curl_easy_init();

		if (curl_handle) {
			curl_easy_setopt(curl_handle, CURLOPT_USERNAME, username);
			curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, password);
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, empty_callback);
			curl_easy_setopt(curl_handle, CURLOPT_URL, build_url);
			curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, "true");
			success = curl_easy_perform(curl_handle);
			curl_easy_cleanup(curl_handle);
		}
		if (success == CURLE_HTTP_RETURNED_ERROR)
			return 0;
		else {
			libtwit_twitter_username = malloc(SLENGTH);
			libtwit_twitter_password = malloc(SLENGTH);

			strncpy(libtwit_twitter_username, username, SLENGTH);
			strncpy(libtwit_twitter_password, password, SLENGTH);

			return 1;
		}
	}
	else
		return 0;
}

int 
send_post_update(char *url, char *file, char *in_message)
{
	CURLcode success;
	CURL *curl_handle;
	struct curl_httppost *message = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *slist = NULL;

	char build_url[SLENGTH];
	strcat(build_url, url);
	strcat(build_url, file);

	curl_formadd(&message, &last, CURLFORM_COPYNAME, "status", CURLFORM_COPYCONTENTS, in_message, CURLFORM_END);
	slist = curl_slist_append(slist, "Expect:");

	curl_handle = curl_easy_init();
	
	if (curl_handle) {
		curl_easy_setopt(curl_handle, CURLOPT_USERNAME, libtwit_twitter_username);
		curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, libtwit_twitter_password);
		curl_easy_setopt(curl_handle, CURLOPT_URL, build_url);
		curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, message);
		curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);
		success = curl_easy_perform(curl_handle);

		curl_easy_cleanup(curl_handle);
		curl_formfree(message);
	}
	if (!success)
		return 1;
	else
		return 0;
}

static size_t
xml_write_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct xml_memory *mem = (struct xml_memory *)data;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

/**
 * There must be a more sane way to do this with libcurl, but I couldn't seem to find a way to supress all data writing.
 */
static size_t
empty_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
	return 0;
}

struct 
xml_memory *retrieve_xml_file(char *file)
{
	CURLcode success;
	CURL *curl_handle;
	struct xml_memory *mem = malloc(sizeof(struct xml_memory));

	char build_url[SLENGTH];
	strcat(build_url, STATUS_URL);
	strcat(build_url, file);

	curl_handle = curl_easy_init();
	if (curl_handle) {
		curl_easy_setopt(curl_handle, CURLOPT_USERNAME, libtwit_twitter_username);
		curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, libtwit_twitter_password);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, xml_write_callback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)mem);
		curl_easy_setopt(curl_handle, CURLOPT_URL, build_url);
		success = curl_easy_perform(curl_handle);

		curl_easy_cleanup(curl_handle);
	}
	if (!success) {
		printf("Successful login!\n");
		return mem;
	}
	else
		return NULL;
}

struct tweet 
*parse_tweet_doc(char *tweet_doc)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	struct tweet *starting_tweet;
	struct xml_memory *mem;
	mem = retrieve_xml_file(tweet_doc);

	if (mem) {
		doc = open_xml_file(mem);
		cur = xmlDocGetRootElement(doc);
		starting_tweet = parse_tweets(cur);
		xmlFreeDoc(doc);
		free(mem);
		
		return starting_tweet;
	}
	else
		return NULL;
}

