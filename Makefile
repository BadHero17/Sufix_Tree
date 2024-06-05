CFLAGS = -Werror -Wall -g

.PHONY: build run clean

build : main.c functions.c
	gcc -o sufix main.c functions.c $(CFLAGS)

run : sufix
	./sufix

clean : sufix
	rm -rf sufix