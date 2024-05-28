CC = g++
CFLAGS = -Wall -Wextra
LDFLAGS = `pkg-config --cflags --libs opencv4`

.PHONY: all clean

all: main

main: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	mkdir -p images

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm -f main main.o
	rm -rf images

