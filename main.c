#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

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

size_t translateOutput (char *data, size_t size, size_t nmemb, void *userData) {
    static int callCount = 0;

    if (callCount == 0) {
        char *findStr = strstr (data, "Перевод");
        findStr = strtok (findStr, ".");
        puts (findStr);
    }

    callCount++;
    return nmemb;
}

void translate (char *word) {
    char link [64];
    sprintf (link, "https://wooordhunt.ru/word/%s", word);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init ();
    curl_easy_setopt(handle, CURLOPT_URL, link);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, translateOutput);

    CURLcode success = curl_easy_perform (handle);

    if (success != CURLE_OK) 
        printf ("%s", curl_easy_strerror(success));

    curl_global_cleanup ();
}

int findElem (char *elem, char *array [], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp (elem, array[i]) == 0) {
            return i;
        }
    }

    return -1;
}

void startProcess (char *prefix, char *command, char *userInputTrimmed) {

    char *prefixes [] = {"b", "bs", "c", "t"};

    switch (findElem (prefix, prefixes, sizeof (prefixes) / 8)) {
        case 0: execlp ("brave", "", command); break;
        case 1: browserQuery(command); break;
        case 2: calcualte(command); break;
        case 3: translate (command); break;
        default: execlp (userInputTrimmed, NULL);
    }
}

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
        /* sleep (1); */ // Play with it for launcher auto-close
        sleep (1); // Works perfect if start cpotlight from terminal manually
        /* getchar (); // Wait user input before exit */


    return EXIT_SUCCESS;
}
