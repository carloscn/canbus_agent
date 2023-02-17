
.PHONY: clean

CC = arm-linux-gnueabihf-gcc
LD = arm-linux-gnueabihf-ld

TARGET=all
SOURCE = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SOURCE))
INCS = -I./             \
	   -I../inc         \
	   -I../thirdpart/openssl/include

all: $(OBJ)
	echo $(OBJ)

%.o : %.c
	$(CC) -c $^ -o $@ $(INCS)

clean:
	rm -rf $(OBJ) *.elf