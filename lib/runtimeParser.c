#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include "../include/runtimeParser.h"
#include "../include/helper.h"


size_t translateOutput (char *data, size_t size, size_t nmemb, void *response) {

    if (strcmp ((char *) response, "") == 0) {
        char *findStr = strstr (data, "Перевод");
        findStr = strtok (findStr, ".");

        strcpy ((char *) response, findStr);
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

void pathParse (char *userInput, char *result [], int resultCount, int resultSize) {
    char findCmd [64] = "";
    sprintf (findCmd, "ls -l /usr/bin | awk '{print $9}' | grep %s", userInput);

    FILE *find = popen (findCmd, "r");

    for (int i = 0; i < resultCount; i++) {
        fgets (result [i], resultSize - 1, find);
    }

    pclose (find);
}

void calculate (char *command, char *result, int resultSize) {
    char calcCmd [32];
    sprintf (calcCmd, "awk \"BEGIN {print %s}\" 2>&1", command);

    FILE *calculations = popen (calcCmd, "r");
    fgets(result, resultSize - 1, calculations);
    pclose (calculations);
}

void parse (char *userInput, char *result [], int resultCount, int resultSize) {
    char *prefixes [] = {"c", "t"};

    char *prefix = strtok (userInput, ";");
    char *command = strtok (NULL, ";");

    if (command) {
        switch (arrFind (prefix, prefixes, sizeof (prefixes) / 8)) {
            case 0: calculate (command, result [0], resultSize); break;
            case 1: translate (command, result [0]); break;
            default: strcpy (result [0], "Wrong command");
        }
    } else if (prefix)
        pathParse (userInput, result, resultCount, resultSize); 
    else
        strcpy (result [0], "Waiting for input");
}
