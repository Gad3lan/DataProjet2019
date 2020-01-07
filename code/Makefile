OBJS = main.cpp
csvReader = csvReader.cpp csvReader.h
curve = curve.cpp curve.h
loadShader = loadShader.cpp loadShader.h

CC = g++ -std=c++11

COMPILER_FLAGS = -Wall

LINKER_FLAGS = -lglfw -lGLEW -framework OpenGL

OBJ_NAME = projet

all: $(OBJS)
	$(CC) $(OBJS) $(csvReader) $(curve) $(loadShader) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean: rm -rf *.o