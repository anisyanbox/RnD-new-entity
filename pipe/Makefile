.PHONY: all clean

EXEC	=solution
SRC	=solution.c
CFLAGS	= -ggdb -o0

all: $(EXEC)

$(EXEC): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $@

clean:
	$(RM) *.o $(EXEC)

