#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

static struct config {
	char	browser [32];
	int	padding;
	int	upKeycode;
	int	downKeycode;
	int	acKeycode;
	int	resultCount;
	char	fileManager [32];
	char	pdfReader [32];
	char	mp4Player [32];
	char	mkvPlayer [32];
	char	xlsxReader [32];
	char	docxReader [32];
} config;

struct config *configParser ();

#endif
