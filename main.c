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

#define _PROMT_ "Enter your command:"

struct winData {
	WINDOW	*win;
	WINDOW	*inputWin;
	char	userInput [128];
	int	inputPos;
	int	spKey;
};

void *parsingThread ( void *data )
{
	struct config	*config		= configParser ();
	struct winData	*pWin		= ( struct winData * ) data;
	WINDOW		*inputWin	= pWin -> inputWin;
	char		userInputCopy [128];
	char		*result [config->resultCount];
	int		selectedResult	= 0;
	int		resultSize	= 256;
	int		findedResults;

	char		*prefix;
	char		*command;

	while ( 1 ) {
		strcpy ( userInputCopy, pWin->userInput );

		usleep ( 10000 ); // 0.01 sec

		if ( strcmp ( userInputCopy, pWin->userInput ) != 0 ) {
			selectedResult = 0;
			continue;
		}

		/*
		   REVIEW:
		   Change to one malloc by linear calculations instead of
		   for-loop. Or maybe even to function call in whole loop?
		 */
		for ( int i = 0; i < config->resultCount; i++ ) {
			result [i] = malloc ( resultSize );
			strcpy ( result [i], "" );
		}

		parse ( userInputCopy, result, config->resultCount, resultSize );
		werase ( pWin->win );

		/*
		   REVIEW:
		   Maybe it's a good idea to change
		   prefix and command vars to an
		   array of 2 elements or a struct
		*/
		prefix	= strtok ( userInputCopy, ";" );
		command = strtok ( NULL, ";" );

		if ( pWin->spKey == KEY_ENTER ) {
			if ( strcmp ( prefix, "f" ) != 0 ) { /* REVIEW: Nested if ?!  */
				strcpy ( pWin->userInput, strtok ( result [selectedResult], "\n" ) );
			}

			break;

		} else if ( pWin->spKey == config->acKeycode ) {

			if ( command && strcmp ( prefix, "f" ) == 0 ) { /* REVIEW: Again?  */
				int commandLen = strlen ( command );

				/*  REVIEW: Change it to for-loop? */
				while ( strcmp ( &command [commandLen], "/" ) != 0 ) {
					command [commandLen--] = '\0';
				}

				/* REVIEW: A little bit difficult to read... */
				sprintf ( pWin->userInput, "%s;%s%s/", prefix, command, strtok ( result [selectedResult], "\n" ) );

			} else {
				/* REVIEW: Same as previous */
				sprintf ( pWin -> userInput, "%s", strtok ( result [selectedResult], "\n" ) );
			}

			werase ( pWin->inputWin );
			box ( pWin->inputWin, 0, 0 );
			mvwprintw ( pWin->inputWin, 1, 1, "%s", pWin->userInput );
			pWin->inputPos = strlen ( pWin->userInput );
			wrefresh ( pWin->inputWin );
		}

		findedResults = 0;

		for ( int i = 0; i < config->resultCount; i++ ) {
			if ( strcmp ( result [i], "" ) != 0 ) {

				if ( i == selectedResult ) {
					wattron ( pWin->win, COLOR_PAIR ( 1 ) );
				}

				mvwprintw ( pWin->win, i + 1, 1, "%i. %s", i + 1, result [i] );

				wattroff ( pWin->win, COLOR_PAIR ( 1 ) );
				findedResults++;
			}

			free ( result [i] );
		}

		if ( pWin->spKey == KEY_UP && selectedResult > 0 ) {
			selectedResult--;
		} else if ( pWin -> spKey == KEY_DOWN && selectedResult < findedResults - 1 ) {
			selectedResult++;
		}

		pWin->spKey = -1;

		box ( pWin->win, 0, 0 );
		wrefresh ( pWin->win );
	}

	return 0;
}

int main ( void )
{
	int		ch;
	int		maxCol;
	int 		maxRow;
	pthread_t	parsing_thr;
	struct winData	parsingWin;
	struct config	*config = configParser ();
	WINDOW		*inputWin;

	initscr ();
	raw ();
	nonl ();

	start_color ();
	init_pair ( 1, COLOR_BLACK, COLOR_WHITE );

	getmaxyx ( stdscr, maxRow, maxCol );

	mvprintw ( 1, ( maxCol / 2 ) - ( strlen ( _PROMT_ ) / 2 ), _PROMT_ );
	refresh ();

	inputWin = newwin ( 3, maxCol - config->padding * 2, 3, config->padding );
	keypad ( inputWin, TRUE );
	box ( inputWin, 0, 0 );
	wrefresh ( inputWin );

	parsingWin.win		= newwin ( maxRow - 8, maxCol - config -> padding * 2, 7, config -> padding );
	parsingWin.inputWin	= inputWin;
	parsingWin.inputPos	= 0;

	pthread_create ( &parsing_thr, NULL, parsingThread, ( void * ) &parsingWin );

	while ( 1 ) {
		wmove ( inputWin, 1, parsingWin.inputPos + 1 );
		ch = wgetch ( inputWin );

		if ( ch == KEY_ENTER || ch == '\r' ) {
			parsingWin.spKey = KEY_ENTER;
			break;

		} else if ( isprint ( ch ) ) {
			parsingWin.userInput [parsingWin.inputPos++] = ch;

		} else if ( ch == 27 ) {
			pthread_cancel ( parsing_thr );
			strcpy ( parsingWin.userInput, "" );
			break;

		} else if ( ch == KEY_BACKSPACE && parsingWin.inputPos > 0 ) {
			parsingWin.userInput [--parsingWin.inputPos] = '\0';
			wclrtoeol ( inputWin );
			box ( inputWin, 0, 0 );

		} else if ( ch == KEY_UP || ch == config -> upKeycode ) {
			parsingWin.spKey = KEY_UP;

		} else if ( ch == KEY_DOWN || ch == config -> downKeycode )
		{ parsingWin.spKey = KEY_DOWN; }

		else if ( ch == config -> acKeycode ) {
			parsingWin.spKey = config -> acKeycode;
		}
	}

	pthread_join ( parsing_thr, 0 );
	startProcess ( parsingWin.userInput );

	endwin ();

	return EXIT_SUCCESS;
}
