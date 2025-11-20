CC = gcc
CFLAGS = -Wall -g
TARGET = messenger.exe
SERVER_TARGET = server.exe

all: $(TARGET)

server: $(SERVER_TARGET)

# Rule for the client executable
$(TARGET): src/main.o src/user.o src/conversation_manager.o
	$(CC) $(CFLAGS) -o $(TARGET) src/main.o src/user.o src/conversation_manager.o

# Rule for the server executable
$(SERVER_TARGET): src/server.o
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) src/server.o -lws2_32

src/main.o: src/main.c src/user.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/user.o: src/user.c src/user.h
	$(CC) $(CFLAGS) -c src/user.c -o src/user.o

src/conversation_manager.o: src/conversation_manager.c src/conversation_manager.h
	$(CC) $(CFLAGS) -c src/conversation_manager.c -o src/conversation_manager.o

src/server.o: src/server.c
	$(CC) $(CFLAGS) -c src/server.c -o src/server.o

clean:
	del src\main.o src\user.o src\conversation_manager.o src\server.o $(TARGET) $(SERVER_TARGET) 2>nul

.PHONY: all clean
