CC = gcc
TARGET = reading
LUSB = -lusb-1.0
OBDIR = obj

.PHONY: clean

$(TARGET): $(OBDIR)/$(TARGET).o
	$(CC) -o $(TARGET) $(OBDIR)/$(TARGET).o $(LUSB)

$(OBDIR)/$(TARGET).o: $(TARGET).c
	@mkdir -p $(OBDIR)
	$(CC) -c -o $(OBDIR)/$(TARGET).o $(TARGET).c

clean:
	rm -f $(TARGET) $(OBDIR)/$(TARGET).o


