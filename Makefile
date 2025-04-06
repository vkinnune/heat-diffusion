CC = mpicc
CFLAGS = -Wall -O2
TARGET = heat

all:
	$(CC) $(CFLAGS) -o $(TARGET) heat.c

clean:
	rm -f $(TARGET)
