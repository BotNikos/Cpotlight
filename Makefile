main: mainLib processLib helperLib runtimeParserLib
	clang -g -o cpotlight bin/main.o bin/process.o bin/runtimeParser.o bin/helper.o -lcurl -lncurses -lpthread

mainLib: main.c bin
	clang -g -c main.c -o bin/main.o

processLib: include/process.h lib/process.c bin
	clang -g -c lib/process.c -o bin/process.o

runtimeParserLib: include/runtimeParser.h lib/runtimeParser.c bin
	clang -g -c lib/runtimeParser.c -o bin/runtimeParser.o

helperLib: include/helper.h lib/helper.c bin
	clang -g -c lib/helper.c -o bin/helper.o

bin:
	mkdir bin

clean: ./bin
	rm -rf ./bin

test: test.c
	clang test.c -g -o test -lcurl -lncurses
