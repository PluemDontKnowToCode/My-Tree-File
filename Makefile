CC		= gcc
CFLAGS  = -Wall -Wextra -O2
TARGET  = my_tree.exe
SRC     = main.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET) : $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean: 
	rm -f $(TARGET)