#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <pthread.h>
#include <ncurses.h>

#include "include/process.h"
#include "include/runtimeParser.h"

typedef struct winData {
    WINDOW * win;
    char userInput [128];
    int arrKey;
} winData;

void *parsingThread (void *data) {

    winData *pWin = (winData *) data;

    int selectedResult = 0;

    while (1 == 1) {
        char userInputCopy [128];
        strcpy (userInputCopy, pWin -> userInput);

        // wait when user complete typing
        usleep(100000); // 0.1 sec
        if (strcmp (userInputCopy, pWin -> userInput) != 0) {
            selectedResult = 0;
            continue;
        }

        int resultCount = 10;
        int resultSize = 32;
        char *result [resultCount];

        for (int i = 0; i < resultCount; i++) {
            result [i] = (i == 0) ? malloc (256) : malloc (resultSize);
            strcpy (result [i], "");
        }

        parse (userInputCopy, result, resultCount, resultSize);

        werase (pWin -> win);
        box (pWin -> win, 0, 0);

        for (int i = 0; i < resultCount; i++) {
            mvwprintw (pWin -> win, i + 1, 1, "%i. %s", i + 1, result [i]);
            free (result [i]);
        }

        if (pWin -> arrKey == 0 && selectedResult > 0) 
            selectedResult -= 1;
        else if (pWin -> arrKey == 1 && selectedResult < 10)
            selectedResult += 1;

        mvwprintw (pWin -> win, 12, 1, "Selected result = %i", selectedResult);
        pWin -> arrKey = -1;

        wrefresh (pWin -> win);
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
    keypad (inputWin, TRUE);
    box (inputWin, 0, 0);
    wrefresh(inputWin);

    pthread_t thread_id;
    parsingWin.win = newwin (maxRow - 8, maxCol - 20, 7, 10);
    pthread_create (&thread_id, NULL, parsingThread, (void *) &parsingWin);

    int ch;
    int pos = 0;
    while (1 == 1) {

        wmove (inputWin, 1, pos + 1);
        ch = wgetch (inputWin);

        if (ch == KEY_ENTER || ch == '\n' || ch == '\r')
            break;
        else if (isprint (ch))
            parsingWin.userInput [pos++] = ch;
        else if (ch == KEY_BACKSPACE && pos > 0) {
            parsingWin.userInput [--pos] = '\0';
            wclrtoeol(inputWin);
        } else if (ch == KEY_UP)
            parsingWin.arrKey = 0;
        else if (ch == KEY_DOWN)
            parsingWin.arrKey = 1;
            
    }

    startProcess (parsingWin.userInput);

    pthread_cancel (thread_id);
    endwin ();

    return EXIT_SUCCESS;
}
