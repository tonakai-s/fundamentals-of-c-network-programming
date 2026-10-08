TARGET = bin/main
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/*.c, obj/%.o, $(SRC))

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	gcc -g -o $@ $? -I./inc -Wall
