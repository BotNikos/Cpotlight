#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
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

        int resultCount = 10;
        int resultSize = 32;
        char *result [resultCount];

        for (int i = 0; i < resultCount; i++) {
            result [i] = (i == 0) ? malloc (256) : malloc (resultSize);
            strcpy (result [i], "");
        }

        parse (userInputCopy, result, resultCount, resultSize);

        box (pWin -> win, 0, 0);

        for (int i = 0; i < resultCount; i++) {
            mvwprintw (pWin -> win, i + 1, 1, "%s", result [i]);
            free (result [i]);
        }

        wrefresh (pWin -> win);
        werase (pWin -> win);
    }

    return 0;
}

int main (void) {
    setlocale(LC_ALL, "");

    int maxCol, maxRow;
    winData parsingWin;
    memset(parsingWin.userInput, '\0', sizeof (parsingWin.userInput));

    initscr ();
    raw ();

    getmaxyx (stdscr, maxRow, maxCol);

    mvprintw (1, (maxCol / 2) - (strlen ("Enter your command:") / 2), "Enter your command:");
    refresh ();

    WINDOW *inputWin = newwin (3, maxCol - 20, 3, 10);
    box (inputWin, 0, 0);
    wrefresh(inputWin);

    pthread_t thread_id;
    parsingWin.win = newwin (maxRow - 8, maxCol - 20, 7, 10);
    pthread_create (&thread_id, NULL, parsingThread, (void *) &parsingWin);

    mvwgetstr (inputWin, 1, 1, parsingWin.userInput);
    startProcess (parsingWin.userInput);

    pthread_cancel (thread_id);
    endwin ();

    return EXIT_SUCCESS;
}
