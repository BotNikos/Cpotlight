cp: main process
	clang -o cpotlight bin/main.o bin/process.o -lcurl

main: main.c bin
	clang -c -g main.c -o bin/main.o

process: include/process.h lib/process.c bin
	clang -c -g lib/process.c -o bin/process.o

bin:
	mkdir bin

test: test.c
	clang test.c -g -o test -lcurl -lncurses
