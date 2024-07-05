#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#include "include/process.h"

void drawInputBorder (int maxCol, int margin) {
    mvaddch (3, margin, '|');
    mvaddch (3, maxCol - margin, '|');

    for (int i = margin; i <= maxCol - margin; i++) {
        mvaddch (2, i, '-');
        mvaddch (4, i, '-');
    }
}

void *testThread (void *thread) {
    struct dt {
        WINDOW *win;
        char userInput [128];
    } *data = (struct dt *) thread;

    while (1 == 1) {

        if (strcmp (data -> userInput, "exit") == 0) {
            break;
        }

        mvwprintw (data -> win, 1, 1, "%s", data -> userInput);
        wrefresh (data -> win);
        werase (data -> win);

        box (data -> win, 0, 0);
        sleep (1);
    }
}


int main (void) {
    int maxCol, maxRow;

    pthread_t thread_id;

    struct thr {
        WINDOW *win;
        char userInput [128];
    } thread;

    initscr ();
    raw ();

    getmaxyx (stdscr, maxRow, maxCol);

    mvprintw (1, (maxCol / 2) - (strlen ("Enter your command:") / 2), "Enter your command:");
    drawInputBorder(maxCol, 10);
    refresh ();


    thread.win = newwin (maxRow - 7, maxCol, 7, 0);

    pthread_create (&thread_id, NULL, testThread, (void *) &thread);

    move (3, 11);
    getstr (thread.userInput);

    /* fgets (userInput, sizeof (userInput), stdin); */

    /* char *userInputTrimmed = strtok (userInput, "\n"); */
    /* char *prefix = strtok (userInputTrimmed, ";"); */
    /* char *command = strtok (NULL, ";"); */

    /* int process = fork (); */

    /* if (process == 0) */
    /*     startProcess (prefix, command, userInputTrimmed); */
    /* else */
    /*     sleep (1); */

    pthread_exit (NULL);
    endwin ();

    return EXIT_SUCCESS;
}
