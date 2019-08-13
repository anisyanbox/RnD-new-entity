.PHONY: all ree_host start_build_host end_build_host directories

CROSS_COMPILER=arm-linux-gnueabihf-
CC=$(CROSS_COMPILER)gcc
CCFLAGS= -Wall -g
LDFLAG=

DEF= -DBINARY_PREFIX="Hello_TA "
COMMON_INC=-I./include/

# Fix path to the libraries for HOST there
REE_EXEC=ree_host_client
REE_EXE_OUT=$(OUT_DIR)/$(REE_EXEC)
REE_APP_DIR=./ree_host
REE_INC_LIST=$(wildcard $(REE_APP_DIR)/*.h)
REE_INC=$(foreach inc, $(REE_INC_LIST), -I$(inc))
REE_SRC=$(wildcard $(REE_APP_DIR)/*.c)
REE_LIBS=-lteec
REE_OBJ_DIR=$(OUT_DIR)/ree_obj
REE_OBJ_PATH=$(REE_OBJ_DIR)/*.o
REE_OBJECTS=$(patsubst %.c, %.o, $(REE_SRC))

all: ree_host end_build_host

ree_host: start_build_host $(REE_OBJECTS) directories
	$(CC) $(LDFLAG) $(REE_OBJ_PATH) $(REE_LIBS) -o $(REE_EXE_OUT)

start_build_host:
	@echo
	@echo "*** Build the ree_host application ***"

%.o: %.c
	$(CC) $(CCFLAGS) $(REE_INC) $(COMMON_INC) $(DEF) -c $*.c -o $*.o

directories:
	@mkdir -p $(REE_OBJ_DIR)
	@mv -f $(REE_OBJECTS) -t $(REE_OBJ_DIR)/

end_build_host:
	@echo "Done."
