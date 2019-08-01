# common variables
PROG_DIR=.
OBJ_DIR=obj
EXE_FILE=crypto-flight

# compiler and its flags
CC=gcc
CFLAGS= -Wall -c -static
LDFLAG=

# linked libraries
LIBS= -lssl -lcrypto -I/usr/include/openssl/

# path to all includes
INCLUDES= -I$(PROG_DIR)/inc/

# find all sources
C_SRC_PATH=$(PROG_DIR)/src
C_SRC_EXTENSION=c
C_SRC=$(wildcard $(C_SRC_PATH)/*.$(C_SRC_EXTENSION))

# do "*.o" files
OBJ_EXTENSION=o
OBJECTS=$(patsubst %.c, %.o, $(C_SRC))
OBJ_PATH=$(OBJ_DIR)/*.$(OBJ_EXTENSION)

# targets
.PHONY: all
all: $(EXE_FILE)

.PHONY: default
default: $(EXE_FILE)

$(EXE_FILE): $(OBJECTS)
	$(CC) $(LDFLAG) $(OBJ_PATH) $(LIBS) -o $(EXE_FILE)

$(OBJECTS): $(C_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	@mv -f -t $(OBJ_DIR)/ $(OBJECTS)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/
	rm -f $(EXE_FILE)

.PHONY: install
install:

.PHONY: uninstall
uninstall:
