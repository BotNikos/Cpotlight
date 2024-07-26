#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <curl/curl.h>

#include "../include/runtimeParser.h"
#include "../include/helper.h"


size_t translateOutput (char *data, size_t size, size_t nmemb, void *response) {

    if (strcmp ((char *) response, "") == 0) {
        char *findStr = strstr (data, "Перевод");
        findStr = strtok (findStr, ".");

        strcpy ((char *) response, (findStr) ? findStr : "Word not found");
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
    sprintf (findCmd, "ls -l /usr/bin | awk '{print $9}' | grep %s 2>&1", userInput);

    FILE *find = popen (findCmd, "r");

    for (int i = 0; i < resultCount; i++) {
        fgets (result [i], resultSize - 1, find);

        if (strstr (result [i], "grep")) {
            strcpy (result [i], "Wrong command"); 
            break;
        }
    }

    pclose (find);
}

void calculate (char *command, char *result, int resultSize) {
    char calcCmd [32];
    sprintf (calcCmd, "awk \"BEGIN {print %s}\" 2>&1", command);

    FILE *calculations = popen (calcCmd, "r");
    fgets(result, resultSize - 1, calculations);

    if (strstr (result, "awk"))
        strcpy (result, "Calculations error");

    pclose (calculations);
}

char *pathConcat (char *nodes [], int nodesCount) {
    char *path = (char *) malloc (128);

    strcpy (path, "/");
    for (int i = 0; i < nodesCount - 1; i++) {
        strcat (path, nodes [i]);
        strcat (path, "/");
    }

    return path;
}

void fileFinder (char *command, char *result [], int resultCount, int resultSize) {
    char commandCopy [128];
    strcpy (commandCopy, command);

    char *nodes [32];
    int nodesCount = 0;

    char *lastNode = strtok (commandCopy, "/");
    while (lastNode != NULL) {
        nodes [nodesCount] = lastNode; 
        lastNode = strtok (NULL, "/");
        nodesCount++;
    }

    char *lsPath = pathConcat (nodes, nodesCount);

    struct dirent **dir;
    int dirCount = scandir (command, &dir, 0, alphasort);

    if (dirCount > 0) {
        
        for (int i = 0; i < resultCount && i < dirCount; i++) {
            strcpy (result [i], dir [i] -> d_name);
            free (dir [i]);
        }

        free (dir);

    } else
        strcpy (result [0], "Wrong directory");
}

void parse (char *userInput, char *result [], int resultCount, int resultSize) {
    char *prefixes [] = {"c", "t", "b", "bs", "yt", "tg", "f"};
    char userInputCopy [256];
    strcpy (userInputCopy, userInput);

    char *prefix = strtok (userInput, ";");
    char *command = strtok (NULL, ";");

    if (command) {
        switch (arrFind (prefix, prefixes, sizeof (prefixes) / 8)) {
            case 0: calculate (command, result [0], resultSize); break;
            case 1: translate (command, result [0]); break;
            case 2: case 3: case 4: case 5:
            strcpy (result [0], userInputCopy); break;
            case 6: fileFinder (command, result, resultCount, resultSize); break;

            default: strcpy (result [0], "Wrong prefix");
        }
    } else if (prefix)
        pathParse (userInput, result, resultCount, resultSize); 
    else
        strcpy (result [0], "Waiting for input");
}
