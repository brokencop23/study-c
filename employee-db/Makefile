TARGET = bin/db
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))


run: clean default
	./$(TARGET) -f test.db -n
	./$(TARGET) -f test.db -a "Danil,Calle clot 58,120"
	./$(TARGET) -f test.db -l

default: $(TARGET)

clean:
	rm -f bin/*
	rm -f obj/*.o
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o: src/%.c
	gcc -c $< -o $@ -Iinclude
