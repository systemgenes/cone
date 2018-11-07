PROJECT=..
INCLUDE=$(PROJECT)/include
BIN=$(PROJECT)/bin
OBJ=$(PROJECT)/obj
CC=gcc
CFLAGS=-I$(INCLUDE)

$(BIN)/cone:main.c
	$(CC) -o $(BIN)/cone main.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(BIN)/* $(OBJ)/*.o
