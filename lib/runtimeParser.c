#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include "../include/runtimeParser.h"
#include "../include/helper.h"

size_t translateOutput (char *data, size_t size, size_t nmemb, void *userData) {
    static int callCount = 0;

    if (callCount == 0) {
        char *findStr = strstr (data, "Перевод");
        findStr = strtok (findStr, ".");
        puts (findStr);
    }

    callCount++;
    return nmemb;
}

void translate (char *word) {
    char link [64];
    sprintf (link, "https://wooordhunt.ru/word/%s", word);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init ();
    curl_easy_setopt(handle, CURLOPT_URL, link);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, translateOutput);

    CURLcode success = curl_easy_perform (handle);

    if (success != CURLE_OK)
        printf ("%s", curl_easy_strerror(success));

    curl_global_cleanup ();
}

char *calculate (char *command, char *result) {
    char calcCmd [32];
    sprintf (calcCmd, "awk \"BEGIN {print %s}\"", command);

    FILE *calculations = popen (calcCmd, "r");
    fgets(result, 31, calculations);
    pclose (calculations);
    return result;
}

char *parse (char *prefix, char *command, char *userInputTrimmed) {
    char *waitPrefixes [] = {"b", "bs"};
    char *realTimePrefixes [] = {"c", "t"};
    char *result = malloc (32);

    switch (arrFind (prefix, realTimePrefixes, sizeof (realTimePrefixes) / 8)) {
        case 0: calculate (command, result); break;
        /* case 1: translate (command); break; */
        default: strcpy (result, "Wrong command");
    }

    return result;
}
