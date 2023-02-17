.PHONY:all clean

CC = arm-linux-gnueabihf-gcc
LD = arm-linux-gnueabihf-ld

TARGET = canbus_agent.elf
SRCS = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRCS))
INCS = -I./             \
	   -I./inc          \
	   -I../thirdpart/openssl/include
LIBS = -lpthread \
       -L ./thirdpart/openssl/lib/ -lcrypto

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
	make -C ./src
	cp -r ./src/*.o ./build

init:
	mkdir -p ./build

tscp:test
	scp -r test_canbus_agent.elf root@$(REMOTE_IP):/home/root/test_canbus_agent.elf

test:
	make clean
	make
	make -C ./tests/
	cp -r ./tests/test_canbus_agent.elf ./

scp:
	scp -r $(TARGET) root@$(REMOTE_IP):/home/root/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	make -C ./src clean
	rm -rf ./build
	rm -rf *.elf
