.PHONY:all clean

TARGET = canbus_agent.elf
TEST_TARGET = test_canbus_agent.elf

CROSS_COMPILE=arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
DEBUG = -g
CFLAGS = $(DEBUG) -Wall -c -fPIC
TEST_CFLAGS = $(DEBUG) -c -fPIC -Wall -DUNIT_TEST -lstrong

TOP_PATH = $(shell pwd)
INC_PATH = -I$(TOP_PATH)/inc
SRC_PATH = $(TOP_PATH)/src
LIB_PATH = -lpthread

DIRS = $(shell find $(SRC_PATH) -maxdepth 3 -type d)
SRCS_C += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS_C = $(patsubst %.c, %.o, $(SRCS_C))

# config openssl
TP_OPENSSL = $(TOP_PATH)/thirdpart/openssl
INC_PATH += -I$(TP_OPENSSL)/include
LIB_PATH += -L$(TP_OPENSSL)/lib -lcrypto

# config unit test
TP_CUNIT = $(TOP_PATH)/thirdpart/cunit
INC_PATH += -I$(TP_CUNIT)/include
TEST_LIB_PATH = $(LIB_PATH)
TEST_LIB_PATH += -L$(TP_CUNIT)/lib -lcunit
TEST_SRC_PATH = $(TOP_PATH)/tests
TEST_INC_PATH = $(INC_PATH)
TEST_INC_PATH += -I$(TOP_PATH)/tests
TEST_INC_PATH += -I$(TP_CUNIT)/include
TEST_DIRS = $(shell find $(TEST_SRC_PATH) -maxdepth 1 -type d)
TEST_SRCS_C += $(foreach dir, $(TEST_DIRS), $(wildcard $(dir)/*.c))
TEST_OBJS_C = $(patsubst %.c, %.o, $(TEST_SRCS_C))

all:$(TARGET)

$(TARGET): $(OBJS_C)
	@ $(CC) $^ -o $@ $(LIB_PATH)
	@ echo Create $(TARGET) ok...

$(OBJS_C): %.o : %.c
	@ $(CC) $(CFLAGS) $< -o $@ $(INC_PATH)

scp:
	scp -r $(TARGET) root@$(REMOTE_IP):/home/root/$(TARGET)

test:$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS_C) $(OBJS_C)
	@ $(CC) $^ $(TEST_INC_PATH) $(TEST_LIB_PATH)  -o $@
	@ echo Create $(TEST_TARGET) ok...

$(TEST_OBJS_C): %.o : %.c
	@ $(CC) $(TEST_CFLAGS) $(TEST_INC_PATH) $< -o $@

tscp: test
	scp -r $(TEST_TARGET) root@$(REMOTE_IP):/home/root/$(TEST_TARGET)

clean:
	@ $(RM) $(TARGET) $(OBJS_C)
	@ $(RM) $(TEST_TARGET) $(TEST_OBJS_C)