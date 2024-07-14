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

void startProcess (char *userInput) {
    char *prefixes [] = {"b", "bs"};

    char *prefix = strtok (userInput, ";");
    char *command = strtok (NULL, ";");

    pid_t process = fork ();

    if (process == 0) {
        setsid ();

        close (STDIN_FILENO);
        fclose (stdout);
        fclose (stderr);

        switch (arrFind (prefix, prefixes, sizeof (prefixes) / 8)) {
            case 0: execlp ("brave", "", command); break;
            case 1: browserQuery(command); break;
            default: execlp (userInput, NULL);
        }
    }
}
