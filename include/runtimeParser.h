#ifndef RUNTIME_PARSER_H
#define RUNTIME_PARSER_H

size_t translateOutput (char *data, size_t size, size_t nmemb, void *userData) ;
void translate (char *word, char *result);
void pathParse (char *userInput, char *result [], int resultCount, int resultSize);
void calcualte (char *command, char *result, int resultSize);
void parse (char *userInput, char *result [], int resultCount, int resultSize);

#endif
