CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic

main: main.c
	${CC} ${CFLAGS} $@.c -o $@

.PHONY: run
run: main
	./main

.PHONY: debug
debug: CFLAGS += -g
debug: clean main
	gdb

.PHONY: clean
clean:
	rm main

