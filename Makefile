CC = gcc

CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC = $(wildcard src/*.c)

OBJ = $(SRC:.c=.o)

LIBS = -lncurses -ltinfo

TARGET = sysmon

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(TARGET)

clean:
	rm -f src/*.o $(TARGET)
