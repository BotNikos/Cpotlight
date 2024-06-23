main: main.c
	clang main.c -g -o cpotlight -lcurl

test: test.c
	clang test.c -g -o test -lcurl
