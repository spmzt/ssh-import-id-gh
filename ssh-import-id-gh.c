#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strncat, strcpy */
#include <curl/curl.h>

void error(const char * msg) { perror(msg); exit(0); }
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int main(int argc,char *argv[])
{
    if (argc < 2) { puts("Parameters: <Github Username>"); exit(0); }

    // int i;
    static const char *authorized_keys = "authorized_keys";
    int portno = 80;
    char *host = "github.com";
    char *username = argv[1];
    char githubKeysPathExtention[6] = ".keys";
    char *url;
    url = (char*)malloc(7 + 10 + 1 + 39 + 5 + 1);
    strcpy(url, "https://");                        /* define HTTPS for github */
    strncat(url, host, 10);
    strncat(url, "/", 1);
    strncat(url, username, 39);
    strncat(url, githubKeysPathExtention,5);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        /* open the file */
        FILE *authorized_keys_file;
        authorized_keys_file = fopen(authorized_keys, "wb");

        if(authorized_keys_file) {
        
            /* write the page body to this file handle */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, authorized_keys_file);
        
            /* get it! */
            curl_easy_perform(curl);
        
            /* close the header file */
            fclose(authorized_keys_file);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
