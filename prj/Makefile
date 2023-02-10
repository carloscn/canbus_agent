.PHONY:all clean

CC = arm-linux-gnueabihf-gcc
LD = arm-linux-gnueabihf-ld

TARGET = canbus_agent.elf
SRCS = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRCS))
INCS = -I./             \
	   -I./inc          \
	   -I./thirdpart/inc
LIBS = -lpthread

$(TARGET) : entry

entry: main
	$(CC) ./build/*.o -o $(TARGET) $(INCS) -L./build/ $(LIBS)
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

scp:
	scp -r $(TARGET) root@$(REMOTE_IP):/home/root/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	make -C ./src clean
	rm -rf ./build