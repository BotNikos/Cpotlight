#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *browser = "brave"; // Change browser, require recompilation

void browserQuery (char *command) {
    char searchQuery [128];
    sprintf (searchQuery, "https://google.com/search?q=%s", command);
    execlp (browser, "", searchQuery, NULL);
}

void calcualte (char *command) {
    char calcCmd [30];
    sprintf (calcCmd, "BEGIN {print %s}", command);
    execlp ("awk", "", calcCmd, NULL);
}

void startProcess (char *prefix, char *command, char *userInputTrimmed) {
    if (strcmp (prefix, "b") == 0)
        execlp ("brave", "", command);

    else if (strcmp (prefix, "bs") == 0) 
        browserQuery(command);

    else if (strcmp (prefix, "c") == 0) 
        calcualte(command);

    else
        execlp (userInputTrimmed, NULL);
}

int main (void) {
    char userInput [128];

    puts ("Enter your command:");
    fgets (userInput, sizeof (userInput), stdin);

    char *userInputTrimmed = strtok (userInput, "\n");
    char *prefix = strtok (userInputTrimmed, ";");
    char *command = strtok (NULL, ";");

    int process = fork ();

    if (process == 0) {
        startProcess (prefix, command, userInputTrimmed);
    } else
        getchar (); // Wait user input before exit


    return EXIT_SUCCESS;
}
