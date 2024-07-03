#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#include "include/process.h"

void drawInputBorder (int maxCol, int margin) {
    mvaddch (2, margin, '|');
    mvaddch (2, maxCol - margin, '|');

    for (int i = margin; i <= maxCol - margin; i++) {
        mvaddch (1, i, '-');
        mvaddch (3, i, '-');
    }
}

int main (void) {
    char userInput [128];
    int maxCol, maxRow;

    pthread_t thread_id;

    initscr ();
    raw ();

    getmaxyx (stdscr, maxRow, maxCol);

    mvprintw (0, (maxCol / 2) - (strlen ("Enter your command:") / 2), "Enter your command:\n");
    drawInputBorder(maxCol, 10);

    refresh ();

    move (2, 11);
    getstr (userInput);

    /* fgets (userInput, sizeof (userInput), stdin); */

    /* char *userInputTrimmed = strtok (userInput, "\n"); */
    /* char *prefix = strtok (userInputTrimmed, ";"); */
    /* char *command = strtok (NULL, ";"); */

    /* int process = fork (); */

    /* if (process == 0) */
    /*     startProcess (prefix, command, userInputTrimmed); */
    /* else */
    /*     sleep (1); */

    endwin ();

    return EXIT_SUCCESS;
}
