CC	= gcc
DEBUG	= -g -DDEBUG
CFLAGS	= -ansi -Wall -std=c99
LFLAGS	= -Wall
OBJS	= Chess.o Multiplayer.o Singleplayer.o GameLogic.o 
DEPS	= Multiplayer.h Singleplayer.h GameLogic.h 
DESIGN  = Chat

clean:
	rm -f Chat Server *.o *.txt 
all: $(DESIGN)
	g++ -c main.cpp -lpthread -o main.o
	g++ -c User.cpp -o User.o
	g++ -c SimpleDataBase.cpp -liostream -lfstream -lstdlib -lstring -o SimpleDataBase.o
	g++ main.o SimpleDataBase.o User.o -lpthread -o Server


Chat: Chat.c GameLogic.o
	gcc -Wall -g -ansi -std=c99 Chat.c Chess.c GameLogic.o -o Chat `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`


GameLogic.o: GameLogic.c GameLogic.h
	$(CC) $(CFLAGS) -c GameLogic.c -o GameLogic.o

