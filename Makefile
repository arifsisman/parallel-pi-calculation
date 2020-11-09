TARGET = build
NAME = taylor
CC = mpicc

ifndef np
override np = 4
endif

all:	run

build:  clean
	mkdir build
	$(CC) -o $(TARGET)/$(NAME) $(NAME).c

run :	clean build
	mpirun -np $(np) $(TARGET)/$(NAME)

clean:
	rm -rf $(TARGET)