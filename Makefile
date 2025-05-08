CC = gcc

CFLAGS = -Wall -g

TARGET = simulator

OBJS = main.o process.o scheduler.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
