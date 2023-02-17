.PHONY:all clean

CC = arm-linux-gnueabihf-gcc
LD = arm-linux-gnueabihf-ld

TARGET = test_canbus_agent.elf
SRCS = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRCS))
INCS = -I./             \
	   -I./inc          \
	   -I../inc         \
	   -I../thirdpart/openssl/include \
	   -I../thirdpart/cunit/include
LIBS = -lpthread \
       -L ../thirdpart/openssl/lib/ -lcrypto \
       -L ../thirdpart/cunit/lib/ -lcunit

$(TARGET) : entry

entry: main
	$(CC) ./build/*.o -o $(TARGET) $(INCS) -L./build/  $(LIBS)
	echo "[done] build finish: $(TARGET)"

main: src
	$(CC) -c $(SRCS) $(INCS)
	cp -r ./*.o ./build

%.o:%.c
	$(CC) -c $^ $(INCS)

src: init
	cp -r ../src/*.o ./build

init:
	mkdir -p ./build

clean:
	rm -f $(OBJS) $(TARGET)
	make -C ./src clean
	rm -rf ./build