CC=g++
CFLAGS=-O0 -fopenmp
LDFLAGS=
TARGET=mm

all: mm.cpp
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(TARGET)
