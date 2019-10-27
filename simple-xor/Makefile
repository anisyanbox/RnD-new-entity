.PHONY: all clean

INC=
SRC= simple_xor.c
CCFLAGS= -O0 -g -DDEBUG_MODE

TARGET=xor_enc.a

all:
	gcc $(CCFLAGS) $(INC) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET) *.o
