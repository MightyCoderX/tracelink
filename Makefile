CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic
LIBS := 

main: main.c
	${CC} ${CFLAGS} $@.c -o $@

run: main
	./main

debug: main
	gdb ./main

clean: main
	rm main

