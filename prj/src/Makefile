
.PHONY: clean

CC = arm-linux-gnueabihf-gcc
LD = arm-linux-gnueabihf-ld

TARGET=all
SOURCE = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SOURCE))
INC = ../inc

all: $(OBJ)
	echo $(OBJ)

%.o : %.c
	$(CC) -c $^ -o $@ -I $(INC)

clean:
	rm -rf $(OBJ) *.elf