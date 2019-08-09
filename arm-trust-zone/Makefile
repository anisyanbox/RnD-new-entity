.PHONY: all clean ta ree_host

CROSS_COMPILER=arm-linux-gnueabihf-
CC=$(CROSS_COMPILER)gcc
CCFLAGS= -Wall -g

DEF= -DBINARY_PREFIX="Hello_TA "

TA_EXEC=hello_ta
HOST_EXEC=ree_host_client

COMMON_INC=-I./include/

TA_DIR=./ta
TA_INC_LIST=$(wildcard $(TA_DIR)/*.h)
TA_INC=$(foreach inc, $(TA_INC_LIST), -I$(inc))
TA_SRC=$(wildcard $(TA_DIR)/*.c)
TA_LIBS=

REE_APP_DIR=./ree_host
REE_INC_LIST=$(wildcard $(REE_APP_DIR)/*.h)
REE_INC=$(foreach inc, $(REE_INC_LIST), -I$(inc))
REE_SRC=$(wildcard $(REE_APP_DIR)/*.c)
REE_LIBS=-lteec

all: ree_host ta

ree_host: $(REE_SRC)
	@echo "*** Build ree_host application ***"
	$(CC) $(CCFLAGS) $(DEF) $(REE_LIBS) $(REE_INC) $(COMMON_INC) $< -o $(HOST_EXEC)
	@echo "Done."
	@echo

ta: $(TA_SRC)
	@echo "*** Build trusted application ***"
	$(CC) $(CCFLAGS) $(DEF) $(TA_LIBS) $(TA_INC) $(COMMON_INC) $< -o $(TA_EXEC)
	@echo "Done."
	@echo

clean:
	rm -rf $(TA_EXEC) $(HOST_EXEC)
