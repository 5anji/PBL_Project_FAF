CC=g++
CFLAGS=-std=c++17 -Wall -O2 -lsfml-graphics -lsfml-window -lsfml-system
SOURCES=src/main.cpp src/app.cpp
EXECUTABLE=exec.out

build: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean: $(EXECUTABLE)
	rm ./exec.out
