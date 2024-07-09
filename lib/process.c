#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <curl/curl.h>

#include "../include/process.h"

void browserQuery (char *command) {
    char searchQuery [128];
    sprintf (searchQuery, "https://google.com/search?q=%s", command);
    execlp (browser, "", searchQuery, NULL);
}

char *calculate (char *command) {
    char calcCmd [32];
    char *result;
    sprintf (calcCmd, "awk \"BEGIN {print %s}\"", command);

    FILE *calculations = popen (calcCmd, "r");
    fgets(result, 31, calculations);
    return result;
}

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

int findElem (char *elem, char *array [], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp (elem, array[i]) == 0) {
            return i;
        }
    }

    return -1;
}

char *startProcess (char *prefix, char *command, char *userInputTrimmed) {
    // int execution = 0 - wait for user to enter a full command
    // int execution = 1 - execute command in real time

    char *waitPrefixes [] = {"b", "bs"};
    char *realTimePrefixes [] = {"c", "t"};

    /* switch (findElem (prefix, waitPrefixes, sizeof (waitPrefixes) / 8)) { */
    /*     case 0: execlp ("brave", "", command); break; */
    /*     case 1: browserQuery(command); break; */
    /*     default: execlp (userInputTrimmed, NULL); */
    /* } */

    switch (findElem (prefix, realTimePrefixes, sizeof (realTimePrefixes) / 8)) {
        case 0: return calculate (command); break;
        /* case 1: translate (command); break; */
        default: return "Wrong command";
    }
}
