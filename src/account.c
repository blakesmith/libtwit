
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

#include "core.h"
#include "account.h"

int 
libtwit_verify_credentials(char *username, char *password)
{
	if (!libtwit_is_authenticated()) {
		struct xml_memory *mem = malloc(sizeof(struct xml_memory));

		mem->memory = NULL;
		mem->size = 0;

		char build_url[SLENGTH] = ACCOUNT_URL;
		strcat(build_url, VERIFY_CREDENTIALS);

		if (libtwit_curl_handle) {
			curl_easy_setopt(libtwit_curl_handle, CURLOPT_USERNAME, username);
			curl_easy_setopt(libtwit_curl_handle, CURLOPT_PASSWORD, password);
			curl_easy_setopt(libtwit_curl_handle, CURLOPT_WRITEFUNCTION, xml_write_callback);
			curl_easy_setopt(libtwit_curl_handle, CURLOPT_WRITEDATA, (void *)mem);
			curl_easy_setopt(libtwit_curl_handle, CURLOPT_FAILONERROR, 1);
			curl_easy_setopt(libtwit_curl_handle, CURLOPT_URL, build_url);
			libtwit_curl_code = curl_easy_perform(libtwit_curl_handle);
		}
		free(mem->memory);
		free(mem);
		if (libtwit_curl_code == CURLE_OK) {
			libtwit_twitter_username = malloc(SLENGTH);
			libtwit_twitter_password = malloc(SLENGTH);

			strncpy(libtwit_twitter_username, username, SLENGTH);
			strncpy(libtwit_twitter_password, password, SLENGTH);

			return LIBTWIT_OK;
		}
		else
			return LIBTWIT_CREDENTIAL_ERROR; /* User probably typed in the wrong login info. */
	}
	else
		return LIBTWIT_OK;
}

int 
libtwit_is_authenticated()
{
	if (libtwit_twitter_username && libtwit_twitter_password)
		return 1;	
	else
		return 0;
}

