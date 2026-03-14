CC = g++
CFLAGS = -I./glm-1.0.3/glm -IC:/msys64/ucrt64/include
LDFLAGS = -LC:/msys64/ucrt64/lib -lfreeglut -lopengl32 -lglu32 -lglew32
SRC = main.cpp game.cpp resourceManager.cpp shader.cpp sprite_renderer.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean