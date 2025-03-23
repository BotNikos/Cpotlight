#ifndef _RUNTIME_PARSER_H_
#define _RUNTIME_PARSER_H_

size_t translateOutput ( char *data, size_t size, size_t nmemb, void *userData ) ;
void translate ( char *word, char *result );
void pathParse ( char *userInput, char *result [], int resultCount, int resultSize );
void calcualte ( char *command, char *result, int resultSize );
void fileFinder ( char *command, char *result [], int resultCount, int resultSize );
void parse ( char *userInput, char *result [], int resultCount, int resultSize );

#endif
