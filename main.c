#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#include "include/process.h"
#include "include/runtimeParser.h"

typedef struct winData {
    WINDOW * win;
    char userInput [128];
} winData;

void *parsingThread (void *data) {

    winData *pWin = (winData *) data;

    while (1 == 1) {
        char userInputCopy [128];
        strcpy (userInputCopy, pWin -> userInput);

        // wait when user complete typing
        usleep(500000); // 0.5 sec
        if (strcmp (userInputCopy, pWin -> userInput) != 0)
            continue;

        char *prefix = strtok (userInputCopy, ";");
        char *command = strtok (NULL, ";");

        box (pWin -> win, 0, 0);

        mvwprintw (pWin -> win, 1, 1, "You wrote: %s", pWin -> userInput);
        mvwprintw (pWin -> win, 2, 1, "Prefix: %s", prefix);
        mvwprintw (pWin -> win, 3, 1, "Command: %s", command);

        if (command) {
            char *result = parse (prefix, command, userInputCopy);
            mvwprintw (pWin -> win, 4, 1, "Result: %s", result);
            free (result);
        } else
            mvwprintw (pWin -> win, 4, 1, "%s", "Type something :)");

        wrefresh (pWin -> win);
        werase (pWin -> win);
    }

    return 0;
}

int main (void) {
    int maxCol, maxRow;
    winData parsingWin;
    memset(parsingWin.userInput, '\0', sizeof (parsingWin.userInput));

    /* sleep (30); */

    initscr ();
    raw ();
    curs_set(0);

    getmaxyx (stdscr, maxRow, maxCol);

    mvprintw (1, (maxCol / 2) - (strlen ("Enter your command:") / 2), "Enter your command:");
    refresh ();

    WINDOW *inputWin = newwin (3, maxCol - 20, 3, 10);
    box (inputWin, 0, 0);
    wrefresh(inputWin);

    pthread_t thread_id;
    parsingWin.win = newwin (10, maxCol, 7, 0);
    pthread_create (&thread_id, NULL, parsingThread, (void *) &parsingWin);

    mvwgetstr (inputWin, 1, 1, parsingWin.userInput);

    pthread_cancel (thread_id);
    endwin ();

    return EXIT_SUCCESS;
}
