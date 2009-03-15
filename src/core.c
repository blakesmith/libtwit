/* libtwit (C Twitter library)
* (c) Blake Smith
* Project homepage: http://github.com/blakesmith/libtwit/tree/master

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <libxml/parser.h>

#include "core.h"
#include "status.h"

CURL *libtwit_curl_handle;
CURLcode libtwit_curl_code;
char *libtwit_twitter_username;
char *libtwit_twitter_password;

void 
destroy_tweets(struct status *current)
{

	while (current != NULL) {
		struct status *i = current->next;
		free(current->user);
		free(current);
		current = i;
	}
}

void 
*libtwit_init()
{
	curl_global_init(CURL_GLOBAL_ALL);
	libtwit_curl_handle = curl_easy_init();
}

void 
libtwit_deinit()
{
	if (libtwit_twitter_username != NULL && libtwit_twitter_password != NULL) {
		free(libtwit_twitter_username);
		free(libtwit_twitter_password);
	}
	curl_easy_cleanup(libtwit_curl_handle);
	curl_global_cleanup();
}

xmlDocPtr 
open_xml_file(struct xml_memory *mem)
{
	xmlDocPtr doc;
	doc = xmlReadMemory(mem->memory, (int)mem->size, "noname", NULL, XML_PARSE_NOBLANKS);	
	return doc;
}

struct status 
*create_tweet(struct status *previous_node)
{
	struct status *newTweet;
	newTweet = malloc(sizeof(struct status));

	if (newTweet == NULL) {
		printf("Out of memory, dying...");
		libtwit_deinit();
		exit(0);
	}

	newTweet->prev = previous_node;
	return newTweet;
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
	int i;
	char *search_strings[] = {
		"id",
		"name",
		"screen_name",
		"location",
		"description",
		"profile_image_url",
		"url",
		"protected",
		"followers_count"
	};
	xmlChar *stored_node_values[LENGTH(search_strings)];

	for (i = 0; i < LENGTH(search_strings); ++i) {
		stored_node_values[i] = get_node_value(parent, search_strings[i]);
	}

	new_user->id = atoi(stored_node_values[0]);
	new_user->name = stored_node_values[1];
	new_user->screen_name = stored_node_values[2];
	new_user->location = stored_node_values[3];
	new_user->description = stored_node_values[4];
	new_user->profile_image_url = stored_node_values[5];
	new_user->url = stored_node_values[6];
	new_user->prot = sanitize_string_bool(stored_node_values[7]);
	new_user->followers_count = atoi(stored_node_values[8]);

	for (i = 0; i < LENGTH(search_strings); ++i)
		xmlFree(stored_node_values[i]);

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

struct status 
*parse_status(xmlNodePtr cur)
{
	xmlNodePtr children;
	struct status *starting_status;
	struct status *current_status = NULL;
	struct status *previous_status = NULL;

	for (cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status"))) {
			previous_status = current_status; /* First loop this is NULL */
			current_status = create_tweet(previous_status);

			if (previous_status != NULL)
				previous_status->next = current_status;
			else
				starting_status = current_status;

			strptime((char *)get_node_value(cur, "created_at"), "%a %b %d %H:%M:%S +0000 %Y", &(current_status->created_at)); /* Sets the created_at */
			current_status->user = get_user_data(get_node_ptr(cur, "user"));
			current_status->next = NULL;
			current_status->prev = previous_status;
			current_status->id = atoi(get_node_value(cur, "id"));
			current_status->text = get_node_value(cur, "text");
			current_status->source = get_node_value(cur, "source");
			current_status->truncated = sanitize_string_bool(get_node_value(cur, "truncated"));
			current_status->in_reply_to_status_id = atoi(get_node_value(cur, "in_reply_to_status_id"));
			current_status->in_reply_to_user_id = atoi(get_node_value(cur, "in_reply_to_user_id"));
			current_status->favorited = sanitize_string_bool(get_node_value(cur, "favorited"));
			current_status->in_reply_to_screen_name = get_node_value(cur, "in_reply_to_screen_name");
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"error")))
			return NULL;
	}
	return starting_status;
}

void 
display_tweets(struct status *starting_tweet)
{
	struct status *i;

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
send_post_request(char *url, char *file, char *options[][2], int options_length)
{
	int i;
	struct curl_httppost *message = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *slist = NULL;
	struct xml_memory *mem = malloc(sizeof(struct xml_memory));

	mem->memory = NULL;
	mem->size = 0;

	char build_url[SLENGTH];
	strncpy(build_url, url, SLENGTH);
	strcat(build_url, file);

	for (i = 0; i < options_length; ++i) {
		curl_formadd(&message, &last, CURLFORM_COPYNAME, options[i][0], CURLFORM_COPYCONTENTS, options[i][1], CURLFORM_END);
	}
	
	slist = curl_slist_append(slist, "Expect:");
	
	if (libtwit_curl_handle) {
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_USERNAME, libtwit_twitter_username);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_PASSWORD, libtwit_twitter_password);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_URL, build_url);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_FAILONERROR, 1);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_WRITEFUNCTION, xml_write_callback);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_WRITEDATA, (void *)mem);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_HTTPPOST, message);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_HTTPHEADER, slist);
		libtwit_curl_code = curl_easy_perform(libtwit_curl_handle);

		curl_formfree(message);
	}
	free(mem);
	if (libtwit_curl_code == CURLE_OK)
		return LIBTWIT_OK;
	else
		return libtwit_curl_code;
}

extern size_t
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

struct 
xml_memory *send_get_request(char *url, char *file, char *options[][2], int options_length)
{
	int i;
	char parameter[SLENGTH];
	struct xml_memory *mem = malloc(sizeof(struct xml_memory));

	mem->memory = NULL;
	mem->size = 0;

	char build_url[SLENGTH];
	strcpy(build_url, url);
	strcat(build_url, file);
	for (i = 0; i < options_length; ++i) {
		sprintf(parameter, "?%s=%s", options[i][0], options[i][1]);
		strcat(build_url, parameter);
	}

	if (libtwit_curl_handle) {
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_USERNAME, libtwit_twitter_username);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_PASSWORD, libtwit_twitter_password);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_FAILONERROR, 1);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_WRITEFUNCTION, xml_write_callback);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_WRITEDATA, (void *)mem);
		curl_easy_setopt(libtwit_curl_handle, CURLOPT_URL, build_url);
		libtwit_curl_code = curl_easy_perform(libtwit_curl_handle);
	}
	if (libtwit_curl_code == CURLE_OK) {
		return mem;
	}
	else
		return NULL;
}

struct status 
*parse_tweet_doc(char *url, char *tweet_doc, char *options[][2], int options_length)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	struct status *starting_tweet;
	struct xml_memory *mem;
	mem = send_get_request(url, tweet_doc, options, options_length);

	if (mem) {
		doc = open_xml_file(mem);
		cur = xmlDocGetRootElement(doc);
		starting_tweet = parse_status(cur);
		xmlFreeDoc(doc);
		free(mem);
		
		return starting_tweet;
	}
	else
		return NULL;
}

