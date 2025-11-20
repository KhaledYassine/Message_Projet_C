CC = gcc
CFLAGS = -Wall -g
TARGET = messenger.exe

all: $(TARGET)

$(TARGET): src/main.o src/user.o src/conversation_manager.o
	$(CC) $(CFLAGS) -o $(TARGET) src/main.o src/user.o src/conversation_manager.o

src/main.o: src/main.c src/user.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/user.o: src/user.c src/user.h
	$(CC) $(CFLAGS) -c src/user.c -o src/user.o

src/conversation_manager.o: src/conversation_manager.c src/conversation_manager.h
	$(CC) $(CFLAGS) -c src/conversation_manager.c -o src/conversation_manager.o

clean:
	del src\main.o src\user.o src\conversation_manager.o $(TARGET) 2>nul

.PHONY: all clean
