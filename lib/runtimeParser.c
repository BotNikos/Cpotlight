#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include "../include/runtimeParser.h"
#include "../include/helper.h"

size_t translateOutput (char *data, size_t size, size_t nmemb, void *response) {
    char *result = (char *) response;

    if (strcmp (result, "") == 0) {
        char *findStr = strstr (data, "Перевод");
        findStr = strtok (findStr, ".");
        strcpy (result, findStr);
    }

    return nmemb;
}

void translate (char *word, char *result) {
    char link [64];

    sprintf (link, "https://wooordhunt.ru/word/%s", word);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init ();
    curl_easy_setopt(handle, CURLOPT_URL, link);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, translateOutput);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)result);

    CURLcode success = curl_easy_perform (handle);

    if (success != CURLE_OK)
        strcpy (result, "Wrong input");

    curl_global_cleanup ();
}

void calculate (char *command, char *result) {
    char calcCmd [32];
    sprintf (calcCmd, "awk \"BEGIN {print %s}\" 2>&1", command);
    
    FILE *calculations = popen (calcCmd, "r");
    fgets(result, 255, calculations);
    pclose (calculations);
}

char *parse (char *prefix, char *command, char *userInputTrimmed) {
    char *prefixes [] = {"c", "t"};
    char *result = malloc (256);
    memset (result, '\0', 256);

    switch (arrFind (prefix, prefixes, sizeof (prefixes) / 8)) {
        case 0: calculate (command, result); break;
        case 1: translate (command, result); break;
        default: strcpy (result, "Wrong command");
    }

    return result;
}
