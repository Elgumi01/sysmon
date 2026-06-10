CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude -MMD -MP
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)
LIBS = -lncurses -ltinfo
TARGET = sysmon
PREFIX = /usr/local

.PHONY: all clean install uninstall

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(TARGET)

-include $(DEP)

clean:
	rm -f src/*.o src/*.d $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
