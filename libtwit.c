#include <stdio.h>
#include <curl/curl.h>
#include <libxml/xmlreader.h>

#define XML_FILE "blakesmith.xml"

xmlDocPtr open_user_timeline()
{
	xmlDocPtr doc;
	doc = xmlParseFile(XML_FILE);	
	return doc;
}

void parse_user_timeline(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *id;
	xmlNodePtr children;

	cur = cur->xmlChildrenNode;
	
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"status")))
		{
			for (children = cur->children; children != NULL; children = children->next)
			{
				if ((!xmlStrcmp(children->name, (const xmlChar *)"id")))
				{
					id = xmlNodeGetContent(children);
					printf("id: %s\n", id);
					xmlFree(id);
				}
			}
		}
		cur = cur->next;
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

	return 0;
}
