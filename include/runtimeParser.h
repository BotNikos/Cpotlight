#ifndef RUNTIME_PARSER_H
#define RUNTIME_PARSER_H

char *parse (char *prefix, char *command, char *userInput);
void calcualte (char *command);
size_t translateOutput (char *data, size_t size, size_t nmemb, void *userData) ;
void translate (char *word, char *result);

#endif
