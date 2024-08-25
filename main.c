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
#include "include/configParser.h"


typedef struct winData {
    WINDOW *win;
    WINDOW *inputWin;
    char userInput [128];
    int inputPosition;
    int spKey;
} winData;

void *parsingThread (void *data) {

    winData *pWin = (winData *) data;

    int selectedResult = 0;
    
    struct config *config = configParser ();
    int resultCount = config -> resultCount;
    int resultSize = 256;

    WINDOW *inputWin = pWin -> inputWin;

    while (1 == 1) {
        char userInputCopy [128];
        strcpy (userInputCopy, pWin -> userInput);

        // wait when user complete typing
        /* usleep(100000); // 0.1 sec */
        usleep(10000); // 0.01 sec
        if (strcmp (userInputCopy, pWin -> userInput) != 0) {
            selectedResult = 0;
            continue;
        }

        char *result [resultCount];

        for (int i = 0; i < resultCount; i++) {
            result [i] = malloc (resultSize);
            strcpy (result [i], "");
        }


        parse (userInputCopy, result, resultCount, resultSize);
        werase (pWin -> win);

        char *prefix = strtok (userInputCopy, ";");
        char *command = strtok (NULL, ";");

        if (pWin -> spKey == KEY_ENTER) {
            if (strcmp (prefix, "f") != 0)
                strcpy (pWin -> userInput, strtok (result [selectedResult], "\n"));

            break;

        } else if (pWin -> spKey == config -> acKeycode) {

            if (command && strcmp (prefix, "f") == 0) {
                int commandLen = strlen (command);
                while (strcmp (&command [commandLen], "/") != 0)
                    command [commandLen--] = '\0';

                sprintf (pWin -> userInput, "%s;%s%s/", prefix, command, strtok (result [selectedResult], "\n"));

            } else
                sprintf (pWin -> userInput, "%s", strtok (result [selectedResult], "\n"));

            werase (pWin -> inputWin);
            box (pWin -> inputWin, 0, 0);
            mvwprintw (pWin -> inputWin, 1, 1, "%s", pWin -> userInput);
            pWin -> inputPosition = strlen (pWin -> userInput);
            wrefresh (pWin -> inputWin);
        }

        int findedResults = 0;
        for (int i = 0; i < resultCount; i++) {
            if (strcmp (result[i], "") != 0) {

                if (i == selectedResult)
                    wattron (pWin -> win, COLOR_PAIR(1));

                mvwprintw (pWin -> win, i + 1, 1, "%i. %s", i + 1, result [i]);

                wattroff (pWin -> win, COLOR_PAIR(1));
                findedResults++;
            }

            free (result [i]);
        }

        if (pWin -> spKey == KEY_UP && selectedResult > 0) 
            selectedResult -= 1;
        else if (pWin -> spKey == KEY_DOWN && selectedResult < findedResults - 1)
            selectedResult += 1;

        pWin -> spKey = -1;

        box (pWin -> win, 0, 0);
        wrefresh (pWin -> win);
    }

    return 0;
}

int main (void) {
    setlocale(LC_ALL, "");

    int maxCol, maxRow;
    winData parsingWin;
    memset(parsingWin.userInput, '\0', sizeof (parsingWin.userInput));

    struct config *config = configParser ();

    initscr ();
    raw ();
    nonl ();

    start_color ();
    init_pair (1, COLOR_BLACK, COLOR_WHITE);

    getmaxyx (stdscr, maxRow, maxCol);

    mvprintw (1, (maxCol / 2) - (strlen ("Enter your command:") / 2), "Enter your command:");
    refresh ();

    WINDOW *inputWin = newwin (3, maxCol - config -> padding * 2, 3, config -> padding);
    parsingWin.inputWin = inputWin;
    keypad (inputWin, TRUE);
    box (inputWin, 0, 0);
    wrefresh(inputWin);

    pthread_t thread_id;
    parsingWin.win = newwin (maxRow - 8, maxCol - config -> padding * 2, 7, config -> padding);
    pthread_create (&thread_id, NULL, parsingThread, (void *) &parsingWin);

    int ch;

    parsingWin.inputPosition = 0;
    int *pos = &parsingWin.inputPosition;

    while (1 == 1) {
        wmove (inputWin, 1, (*pos) + 1);
        ch = wgetch (inputWin);

        if (ch == KEY_ENTER || ch == '\r') {
            parsingWin.spKey = KEY_ENTER;
            break;

        } else if (isprint (ch)) {
            parsingWin.userInput [(*pos)++] = ch;

        } else if (ch == 27) {
            pthread_cancel (thread_id);
            strcpy (parsingWin.userInput, "");
            break;

        } else if (ch == KEY_BACKSPACE && (*pos) > 0) {
            parsingWin.userInput [--(*pos)] = '\0';
            wclrtoeol(inputWin);
            box (inputWin, 0, 0);

        } else if (ch == KEY_UP || ch == config -> upKeycode) {
            parsingWin.spKey = KEY_UP;

        } else if (ch == KEY_DOWN || ch == config -> downKeycode)
            parsingWin.spKey = KEY_DOWN;

        else if (ch == config -> acKeycode) {
            parsingWin.spKey = config -> acKeycode;
        }
    }

    pthread_join (thread_id, 0);
    startProcess (parsingWin.userInput);

    endwin ();

    return EXIT_SUCCESS;
}
