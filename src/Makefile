CC=gcc
CFLAGS=-Wall -Werror -Wextra
SOURCE=pong.c
EXENAME=pong

all: compile

compile:
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXENAME)

run: compile
	./$(EXENAME)

clean:
	-rm -rf 
