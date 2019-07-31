.PHONY: all clean

CC=gcc

CFLAGS= -O0 -lcrypto -lssl -I ./inc/
LDFLAGS=

SOURCES_PATH= ./src
SOURCES_EXTENSION=c
SOURCES=$(shell find $(SOURCES_PATH) -name *.$(SOURCES_EXTENSION))

BUILD_FOLDER=build
TARGET=openssl-flight

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -rf $(TARGET) *.o
