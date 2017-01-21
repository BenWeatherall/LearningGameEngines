# Keep this for future
CC=g++
LDFLAGS=-lGLEW -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -std=c++11
CFLAGS=-std=c++11
SOURCES=ObjectLoader.cpp ShaderLoader.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE := main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o: #M make object files
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm $(EXECUTABLE) *.o
