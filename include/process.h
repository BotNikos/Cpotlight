#ifndef PROCESS_H 
#define PROCESS_H

static const char *browser = "brave";

char *startProcess (char *prefix, char *command, char *userInputTrimmed);
void browserQuery (char *command);

#endif
 
