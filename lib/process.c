#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/process.h"
#include "../include/helper.h"
#include "../include/configParser.h"

void startFile (char *command) {
    struct config *config = configParser ();
    char commandCopy [128];
    strcpy (commandCopy, command);

    if (strstr (command, ".")) {
        char *format = strtok (commandCopy, ".");
        format = strtok (NULL, ".");

        if (strcmp (format, "pdf") == 0)
            execlp (config -> pdfReader, "", command, NULL);
        else if (strcmp (format, "mp4"))
            execlp (config -> mp4Player, "", command, NULL);
        else if (strcmp (format, "mkv"))
            execlp (config -> mkvPlayer, "", command, NULL);
        else if (strcmp (format, "xlsx"))
            execlp (config -> xlsxReader, "", command, NULL);
        else if (strcmp (format, "docx"))
            execlp (config -> docxReader, "", command, NULL);

    } else
        execlp (config -> fileManager, "", command, NULL);

}

void startProcess (char *userInput) {
    struct config *config = configParser ();
    char *prefixes [] = {"b", "bs", "yt", "tg", "f"};

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
            case 4: startFile (command); break;
        }

        if (strcmp (link, "") == 0)
            execlp (userInput, NULL);
        else
            execlp (config -> browser, "", link, NULL);
            
    }
}
