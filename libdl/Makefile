.PHONY: all clean

all: lib
	gcc solution.c -ldl -Wl,-rpath=. -o solution

lib:
	gcc libshared.c -fPIC -shared -o libshared.so

clean:
	rm -rf solution *.so
