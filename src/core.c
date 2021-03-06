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
destroy_statuses(struct status *current)
{
	while (current != NULL) {
		struct status *i = current->next;
		destroy_status_data(current);
		destroy_basic_user_data(current->user);
		free(current->user);
		free(current);
		current = i;
	}
}

void 
destroy_basic_user_list(struct basic_user *current)
{

	while (current != NULL) {
		struct basic_user *i = current->next;
		destroy_status_data(current->status);
		destroy_basic_user_data(current);
		free(current->status);
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

struct status 
*create_status()
{
	struct status *newTweet;
	newTweet = malloc(sizeof(struct status));

	if (newTweet == NULL) {
		printf("Out of memory, dying...");
		libtwit_deinit();
		exit(0);
	}

	return newTweet;
}

struct basic_user 
*create_basic_user()
{
	struct basic_user *newUser;
	newUser = malloc(sizeof(struct basic_user));

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
	return NULL;
}

struct status
*get_status_data(xmlNodePtr parent)
{
	if (parent == NULL)
		return NULL;

	struct status *current_status = create_status();
	int i;
	char *search_strings[] = {
		"created_at",
		"id",
		"text",
		"source",
		"truncated",
		"in_reply_to_user_id",
		"in_reply_to_status_id",
		"favorited",
		"in_reply_to_screen_name"
	};

	/* Parse all strings from the xml document */
	for (i = 0; i < STATUS_LENGTH; ++i) {
		current_status->stored_node_ptr[i] = get_node_value(parent, search_strings[i]);
	}

	/* Now put them into the structure with their proper type */
	strptime((char *)current_status->stored_node_ptr[0], "%a %b %d %H:%M:%S +0000 %Y", &(current_status->created_at)); /* Sets the created_at */
	current_status->user = get_basic_user_data(get_node_ptr(parent, "user"));
	current_status->id = atoi(current_status->stored_node_ptr[1]);
	current_status->text = current_status->stored_node_ptr[2];
	current_status->source = current_status->stored_node_ptr[3];
	current_status->truncated = sanitize_string_bool(current_status->stored_node_ptr[4]);
	current_status->in_reply_to_status_id = atoi(current_status->stored_node_ptr[5]);
	current_status->in_reply_to_user_id = atoi(current_status->stored_node_ptr[6]);
	current_status->favorited = sanitize_string_bool(current_status->stored_node_ptr[7]);
	current_status->in_reply_to_screen_name = current_status->stored_node_ptr[8];
	current_status->next = NULL;
	current_status->prev = NULL;

	return current_status;
}


struct basic_user 
*get_basic_user_data(xmlNodePtr parent)
{
	if (parent == NULL)
		return NULL;

	struct basic_user *new_user = create_basic_user();
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

	for (i = 0; i < LENGTH(search_strings); ++i) {
		new_user->stored_node_ptr[i] = get_node_value(parent, search_strings[i]);
	}

	new_user->status = get_status_data(get_node_ptr(parent, "status"));
	new_user->id = atoi(new_user->stored_node_ptr[0]);
	new_user->name = new_user->stored_node_ptr[1];
	new_user->screen_name = new_user->stored_node_ptr[2];
	new_user->location = new_user->stored_node_ptr[3];
	new_user->description = new_user->stored_node_ptr[4];
	new_user->profile_image_url = new_user->stored_node_ptr[5];
	new_user->url = new_user->stored_node_ptr[6];
	new_user->prot = sanitize_string_bool(new_user->stored_node_ptr[7]);
	new_user->followers_count = atoi(new_user->stored_node_ptr[8]);

	return new_user;
}

void
destroy_basic_user_data(struct basic_user *user)
{
	int i;

	for (i = 0; i <  USER_LENGTH; ++i)
		xmlFree(user->stored_node_ptr[i]);
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
	int i;
	xmlNodePtr children;
	struct status *starting_status;
	struct status *current_status = NULL;
	struct status *previous_status = NULL;
	/* If the root element of the xml document isn't a status element, keep stepping through it until it is */
	while (xmlStrcmp(cur->name, (const xmlChar *)"status"))
		if (cur->xmlChildrenNode == NULL)
			return NULL;
		else
			cur = cur->xmlChildrenNode;

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status"))) {
			previous_status = current_status; /* First loop this is NULL */
			current_status = get_status_data(cur);

			if (previous_status != NULL)
				previous_status->next = current_status;
			else
				starting_status = current_status;

			current_status->next = NULL;
			current_status->prev = previous_status;

			cur = cur->next;
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"error")))
			return NULL;
	}
	return starting_status;
}

struct basic_user 
*parse_basic_user(xmlNodePtr cur)
{
	int i;
	xmlNodePtr children;
	struct basic_user *starting_basic_user;
	struct basic_user *current_basic_user = NULL;
	struct basic_user *previous_basic_user = NULL;
	/* If the root element of the xml document isn't a status element, keep stepping through it until it is */
	while (xmlStrcmp(cur->name, (const xmlChar *)"user"))
		if (cur->xmlChildrenNode == NULL)
			return NULL;
		else
			cur = cur->xmlChildrenNode;

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"user"))) {
			previous_basic_user = current_basic_user; /* First loop this is NULL */
			current_basic_user = get_basic_user_data(cur);

			if (previous_basic_user != NULL)
				previous_basic_user->next = current_basic_user;
			else
				starting_basic_user = current_basic_user;

			current_basic_user->next = NULL;
			current_basic_user->prev = previous_basic_user;

			cur = cur->next;
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"error")))
			return NULL;
	}
	return starting_basic_user;
}

void
destroy_status_data(struct status *current_status)
{
	int i;

	for (i = 0; i < STATUS_LENGTH; ++i)
		xmlFree(current_status->stored_node_ptr[i]);
}

struct xml_memory 
*send_post_request(char *url, char *file, char *options[][2], size_t options_length)
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
		curl_slist_free_all(slist);
	}
	if (libtwit_curl_code == CURLE_OK)
		return mem;
	else
		return NULL;
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

struct xml_memory 
*send_get_request(char *url, char *file, char *options[][2], size_t options_length)
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

xmlDocPtr
send_http_request(int type, char *url, char *tweet_doc, char *options[][2], size_t options_length)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	struct status *starting_tweet;
	struct xml_memory *mem;
	if (type == GET_REQUEST)
		mem = send_get_request(url, tweet_doc, options, options_length);
	else if (type == POST_REQUEST)
		mem = send_post_request(url, tweet_doc, options, options_length);
	else
		mem = NULL;

	if (mem) {
		doc = xmlReadMemory(mem->memory, (int)mem->size, "noname", NULL, XML_PARSE_NOBLANKS);
		free(mem->memory);
		free(mem);

		return doc;
	}
	else
		return NULL;
}


struct status 
*parse_status_doc(int type, char *url, char *tweet_doc, char *options[][2], size_t options_length)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	struct status *starting_tweet;

	doc = send_http_request(type, url, tweet_doc, options, options_length);
	cur = xmlDocGetRootElement(doc);
	starting_tweet = parse_status(cur);
	
	xmlFreeDoc(doc);

	return starting_tweet;
}

struct basic_user 
*parse_basic_user_doc(int type, char *url, char *tweet_doc, char *options[][2], size_t options_length)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	struct basic_user *starting_user;

	doc = send_http_request(type, url, tweet_doc, options, options_length);
	cur = xmlDocGetRootElement(doc);
	starting_user = parse_basic_user(cur);
	
	xmlFreeDoc(doc);

	return starting_user;
}

