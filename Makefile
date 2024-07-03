main: mainLib processLib
	clang -o cpotlight bin/main.o bin/process.o -lcurl -lncurses -lpthread

mainLib: main.c bin
	clang -c -g main.c -o bin/main.o

processLib: include/process.h lib/process.c bin
	clang -c -g lib/process.c -o bin/process.o

bin:
	mkdir bin

test: test.c
	clang test.c -g -o test -lcurl -lncurses
