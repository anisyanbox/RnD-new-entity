.PHONY: all clean

EXEC=solution
SRC=solution.c
CFLAGS=-DDEBUG_PROC -ggdb -o0

all: $(EXEC)

$(EXEC): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $@

clean:
	$(RM) *.o $(EXEC)

