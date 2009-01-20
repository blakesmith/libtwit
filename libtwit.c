#include <stdio.h>
#include <curl/curl.h>

int main()
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
