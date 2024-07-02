#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "include/process.h"

int main (void) {
    char userInput [128];

    puts ("Enter your command:");
    fgets (userInput, sizeof (userInput), stdin);

    char *userInputTrimmed = strtok (userInput, "\n");
    char *prefix = strtok (userInputTrimmed, ";");
    char *command = strtok (NULL, ";");

    int process = fork ();

    if (process == 0)
        startProcess (prefix, command, userInputTrimmed);
    else
        sleep (1);

    return EXIT_SUCCESS;
}
