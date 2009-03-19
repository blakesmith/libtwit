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

#ifndef LIBTWIT_CORE_H
#define LIBTWIT_CORE_H
#include <libxml/parser.h>
#include <curl/curl.h>
#include <time.h>

/* Macros */
#define LENGTH(x)	(sizeof(x) / sizeof(x[0]))

/* Definitions */

/* Status */
#define STATUS_URL "http://twitter.com/statuses/"
#define PUBLIC_TIMELINE "public_timeline.xml"
#define FRIENDS_TIMELINE "friends_timeline.xml"
#define USER_TIMELINE "user_timeline.xml"
#define UPDATE "update.xml"

/* Account */
#define ACCOUNT_URL "http://twitter.com/account/"
#define VERIFY_CREDENTIALS "verify_credentials.xml"

#define SLENGTH 256
#define USER_LENGTH 9
#define STATUS_LENGTH 9

extern enum {
	/**
	 * Start our twitter return codes at 200 so we can use them from
	 * functions that may return a CURLcode as well as an internal 
	 * libtwit code.
	 */
	LIBTWIT_OK = 200,
	LIBTWIT_CREDENTIAL_ERROR,
	LIBTWIT_MESSAGE_TOO_LONG,
	LIBTWIT_TRANSMISSION_ERROR,
} libtwit_code;


struct basic_user
{
	int id;
	xmlChar *name;
	xmlChar *screen_name;
	xmlChar *location;
	xmlChar *description;
	xmlChar *profile_image_url;
	xmlChar *url;
	int prot;
	int followers_count;
	xmlChar *stored_node_ptr[USER_LENGTH];
};

struct status
{
	struct status *prev;
	struct status *next;
	struct tm created_at;
	int id;
	xmlChar *text;
	xmlChar *source;
	int truncated;
	int in_reply_to_status_id;
	int in_reply_to_user_id;
	int favorited;
	xmlChar *in_reply_to_screen_name;
	struct basic_user *user;
	xmlChar *stored_node_ptr[STATUS_LENGTH];
};

struct xml_memory
{
	char *memory;
	size_t size;
};

extern CURL *libtwit_curl_handle;
extern CURLcode libtwit_curl_code;
extern char *libtwit_twitter_username;
extern char *libtwit_twitter_password;

void destroy_statuses(struct status *current);

void *libtwit_init();

void libtwit_deinit();

xmlDocPtr open_xml_file(struct xml_memory *mem);

struct status *create_status(struct status *previous_node);

struct basic_user *create_basic_user();

xmlChar *get_node_value(xmlNodePtr parent, char *search_string);

xmlNodePtr get_node_ptr(xmlNodePtr parent, char *search_string);

struct basic_user *get_basic_user_data(xmlNodePtr parent);

void destroy_basic_user_data(struct basic_user *user);

int sanitize_string_bool(xmlChar *test_string);

struct status *parse_status(xmlNodePtr cur);

void destroy_status_data(struct status *current_status);

int send_post_request(char *url, char *file, char *options[][2], int options_length);

extern size_t xml_write_callback(void *ptr, size_t size, size_t nmemb, void *data);

struct xml_memory *send_get_request(char *url, char *file, char *options[][2], int options_length);

struct status *parse_status_doc(char *url, char *tweet_doc, char *options[][2], int options_length);




#endif
