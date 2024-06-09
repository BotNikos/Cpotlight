#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// TODO:
// Запускать программы через exec, вместо system
// для того, чтобы запустить программу таким образом
// нужно передеать ей в качестве аргумента строковой
// литерал, а не массив символов

int main () {
    char command [128];
    char userInput [128];

    char *path = getenv ("PATH");
    char *tempPath = strtok (path, ":");
    char *pathSplited [128];

    int pathCount = 0;
    while (tempPath != NULL) {
        pathSplited [pathCount] = tempPath;
        tempPath = strtok (NULL, ":");
        pathCount++;
    }

    puts ("Enter your command:");
    fgets (userInput, sizeof (userInput), stdin);

    char *prefix = strtok (userInput, ";");
    char *noPrefUserInput = strtok (NULL, ";");

    if (strcmp (prefix, "b") == 0) {
        sprintf (command, "/bin/sh /usr/bin/brave %s", noPrefUserInput);
        system (command);
    } else if (strcmp (prefix, "bs") == 0) {
        sprintf (command, "/bin/sh /usr/bin/brave https://google.com/search?q='%s'", noPrefUserInput);
        system (command);
    } else {
        for (int i = 0; i < pathCount; i++) {
            sprintf (command, "%s/%s", pathSplited [i], userInput);
            printf ("%s", command);
            system (command);
        }
    }

    return 0;
}
