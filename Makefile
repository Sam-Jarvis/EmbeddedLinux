CC = gcc
TARGET = reading
LIBS = lusb-1.0
NAME = reading

.PHONY: clean

$(NAME): $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET) -$(LIBS)

$(NAME).o: $(TARGET).c
	$(CC) -c -o $(TARGET).o $(TARGET).c

clean:
	rm -f $(TARGET) $(TARGET).o
