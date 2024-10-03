#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/helper.h"
#include "../include/configParser.h"

struct config *configParser () {

    char *home = getenv ("HOME");
    char cfgPath [128];
    sprintf (cfgPath, "%s/cpotlight/config.cfg", home);

    FILE *configFile = fopen (cfgPath, "r");

    char *optionList [] = {
        "browser",
        "padding",
        "upKeycode",
        "downKeycode",
        "acKeycode",
        "resultCount",
        "fileManager",
        ".pdf",
        ".mp4",
        ".mkv",
        ".xlsx",
        ".docx",
    };

    for (char str [64]; fgets (str, 64, configFile);) {
        if (strstr (str, "#"))
            continue;
        else {
            char *option = strtok (str, "=");
            char *property = strtok (NULL, "=");
            property = strtok (property, "\n");

            switch (arrFind (option, optionList, sizeof (optionList) / 8)) {
                case 0: strcpy (config.browser, property); break;
                case 1: config.padding = atoi (property); break;
                case 2: config.upKeycode = atoi (property); break;
                case 3: config.downKeycode = atoi (property); break;
                case 4: config.acKeycode = atoi (property); break;
                case 5: config.resultCount = atoi (property); break;
                    // file formats
                case 6: strcpy (config.fileManager, property); break;
                case 7: strcpy (config.pdfReader, property); break;
                case 8: strcpy (config.mp4Player, property); break;
                case 9: strcpy (config.mkvPlayer, property); break;
                case 10: strcpy (config.xlsxReader, property); break;
                case 11: strcpy (config.docxReader, property); break;
            }
        }
    }

    return &config;
}
