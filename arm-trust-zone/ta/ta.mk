.PHONY: all ta start_build_ta end_build_ta directories

CROSS_COMPILER=arm-linux-gnueabihf-
CC=$(CROSS_COMPILER)gcc
CCFLAGS_TA= -Wall -g -fPIC
LDFLAG=

DEF= -DBINARY_PREFIX="Hello_TA "
COMMON_INC=-I./include/

# Fix path to the libraries for HOST there
TA_EXEC=ta
TA_EXE_OUT=$(OUT_DIR)/$(TA_EXEC)
TA_APP_DIR=./ta
TA_INC_DIR=$(TA_APP_DIR)/include/
TA_INC_DIR+=$(TA_APP_DIR)/include/tee_internal_api/
TA_INC_DIR+=$(TA_APP_DIR)/include/tee_internal_api/mbedtls/
TA_INC_DIR+=$(TA_APP_DIR)/include/mbedtls/tee_internal_api/sys/
TA_INC_LIST=$(wildcard $(TA_APP_DIR)/*.h)
TA_INC_LIST+=$(wildcard $(TA_INC_DIR)/*.h)
TA_INC=$(foreach inc, $(TA_INC_LIST), -I$(inc))
TA_SRC=$(wildcard $(TA_APP_DIR)/*.c)
TA_LIBS= -lmbedtls -lutee -lutils
TA_OBJ_DIR=$(OUT_DIR)/ta_obj
TA_OBJ_PATH=$(TA_OBJ_DIR)/*.o
TA_OBJECTS=$(patsubst %.c, %.o, $(TA_SRC))

all: ta

ta: start_build_ta $(TA_OBJECTS) directories end_build_ta

start_build_ta:
	@echo
	@echo "*** Build the trusted application ***"

%.o: %.c
	$(CC) $(CCFLAGS_TA) $(TA_INC) $(COMMON_INC) $(DEF) -c $*.c -o $*.o

directories:
	@mkdir -p $(TA_OBJ_DIR)
	@mv -f $(TA_OBJECTS) -t $(TA_OBJ_DIR)/

end_build_ta:
	@echo "Done."
