#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

static struct config {
    char browser [32];
    int padding;
    int upKeycode;
    int downKeycode;
    int resultCount;
} config;

struct config *configParser ();

#endif
