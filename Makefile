CC = g++

SRC_PATH = src
BUILD_PATH = build

CXXFLAGS = -Wall
LIB = -lglfw -lGLEW -framework OpenGL
LDFLAGS = -I/usr/local/include/

SRC := src/main.cpp src/curve.cpp src/loadShaders.cpp src/csvReader.cpp
OBJ = $(patsubst $(SRC_PATH)/%.cpp,$(BUILD_PATH)/%.o,$(SRC))

all: $(SRC) $(EXEC)
	
project: $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) $(LDFLAGS) $(CXXFLAGS) -c $< -o $@

clean: 
	rm $(BUILD_PATH)/*.o
	rm $(EXEC)