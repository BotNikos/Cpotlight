#ifndef PROCESS_H 
#define PROCESS_H

static const char *browser = "brave";

char *startProcess (char *prefix, char *command, char *userInputTrimmed);
void browserQuery (char *command);
char *calcualte (char *command);
size_t translateOutput (char *data, size_t size, size_t nmemb, void *userData) ;
void translate (char *word);
int findElem (char *elem, char *array [], int size);

#endif
 
