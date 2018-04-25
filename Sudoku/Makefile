TARGET1 = terminal
TARGET2 = tester

LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Wextra

.PHONY: default all clean

default: $(TARGET1) $(TARGET2)
all: default

OBJECTS1 = terminal.o sudoku.o
OBJECTS2 = tester.o sudoku.o
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET1): $(OBJECTS1)
	$(CC) $(OBJECTS1) -Wall -Wextra $(LIBS) -o $@

$(TARGET2): $(OBJECTS2)
	$(CC) $(OBJECTS2) -Wall -Wextra $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET1)
	-rm -f $(TARGET2)