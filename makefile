CC = g++
FLAGS = -I .
SRC =

OBJ = $(SRC:.cpp=.o)

run:
	make main
	./main

main: main.cpp makefile $(OBJ)
	$(CC) $(FLAGS) -o main main.cpp $(OBJ) 

%.o: %.cpp makefile
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf main
	rm -rf %.o