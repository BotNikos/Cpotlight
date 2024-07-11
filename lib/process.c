#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/process.h"
#include "../include/helper.h"

void browserQuery (char *command) {
    char searchQuery [128];
    sprintf (searchQuery, "https://google.com/search?q=%s", command);
    execlp (browser, "", searchQuery, NULL);
}

char *startProcess (char *prefix, char *command, char *userInputTrimmed) {
    char *waitPrefixes [] = {"b", "bs"};
    char *realTimePrefixes [] = {"c", "t"};
    char *result = malloc (32);

    /* switch (findElem (prefix, waitPrefixes, sizeof (waitPrefixes) / 8)) { */
    /*     case 0: execlp ("brave", "", command); break; */
    /*     case 1: browserQuery(command); break; */
    /*     default: execlp (userInputTrimmed, NULL); */
    /* } */

    return result;
}
