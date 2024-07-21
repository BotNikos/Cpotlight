#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/process.h"
#include "../include/helper.h"

void startProcess (char *userInput) {
    char *prefixes [] = {"b", "bs", "yt", "tg"};

    char *prefix = strtok (userInput, ";");
    char *command = strtok (NULL, ";");

    char link [128];
    memset (link, '\0', sizeof (link));

    pid_t process = fork ();

    if (process == 0) {
        setsid ();

        close (STDIN_FILENO);
        fclose (stdout);
        fclose (stderr);

        switch (arrFind (prefix, prefixes, sizeof (prefixes) / 8)) {
            case 0: strcpy (link, command); break;
            case 1: sprintf (link, "https://google.com/search?q=%s", command); break;
            case 2: sprintf (link, "https://youtube.com/results?search_query=%s", command); break;
            case 3: strcpy (link, "https://web.telegram.org"); break;
        }

        if (strcmp (link, "") == 0)
            execlp (userInput, NULL);
        else
            execlp (browser, "", link, NULL);
            
    }
}
