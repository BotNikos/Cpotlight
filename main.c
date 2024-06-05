#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    char command[30];

    char *path = getenv ("PATH");
    char *tempPath = strtok (path, ":");
    char *pathSplited [128];

    int i = 0;
    while (tempPath != NULL) {
        pathSplited[i] = tempPath;
        tempPath = strtok (NULL, ":");
        i++;
    }

    // TODO: Сделать этот цикл так, чтобы
    // при работе он не выходил за рамки массива
    // pathSplited из-за проверки i < 10

    for (int i = 0; i < 10; i++) {
        printf ("%s\n", pathSplited[i]);
        printf ("%s\n", *(pathSplited + i));
    }

    puts ("Enter your command:");
    scanf ("%s", &command[0]);

    if (strcmp (command, "btop") == 0)
        system (command);
    else
        puts ("Don't know what you talking about");

    return 0;
}
