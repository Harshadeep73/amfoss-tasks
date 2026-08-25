CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = hyprtop

SRC = src/main.c \
      src/process.c \
      src/system.c \
      ui/ui.c \
      src/input.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
