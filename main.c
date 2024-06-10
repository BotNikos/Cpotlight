#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (void) {
    char userInput [128];

    // INFO:
    // $PATH analyzing braks execlp ¯\_(ツ)_/¯ 

    /* char *path = getenv ("PATH"); */
    /* char *tempPath = strtok (path, ":");
    /* char *pathSplited [128]; */

    /* int pathCount = 0; */
    /* while (tempPath != NULL) { */
    /*     pathSplited [pathCount] = tempPath; */
    /*     tempPath = strtok (NULL, ":"); */
    /*     pathCount++; */
    /* } */

    puts ("Enter your command:");
    fgets (userInput, sizeof (userInput), stdin);

    char *userInputTrimmed = strtok (userInput, "\n");
    char *prefix = strtok (userInputTrimmed, ";");
    char *command = strtok (NULL, ";");

    if (strcmp (prefix, "b") == 0)
        execlp ("brave", "", command);

    else if (strcmp (prefix, "bs") == 0) {
        char searchQuery [128];
        sprintf (searchQuery, "https://google.com/search?q=%s", command);
        execlp ("brave", "", searchQuery);

    } else 
        execlp (userInputTrimmed, NULL);

    return EXIT_SUCCESS;
}
