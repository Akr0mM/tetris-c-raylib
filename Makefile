CC = gcc
CFLAGS = -I C:/raylib/include
LDFLAGS = -L C:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -Wl,-subsystem,console

TARGET = tetris
SRC = tetris.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)
