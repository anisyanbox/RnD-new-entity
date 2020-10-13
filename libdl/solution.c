#include <dlfcn.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *libname;
	char *funcname;
	char *funcarg;
	void *hdl;
	int (*secret_func)(int);

	if (argc < 4) {
		perror("args count error");
		return EXIT_FAILURE;
	}

	if (argv[0] == NULL || argv[1] == NULL ||
			argv[2] == NULL || argv[3] == NULL) {
		perror("some of args are <null>");
		return EXIT_FAILURE;
	}

	printf("argc=%d\n[0]: %s\n[1]: %s\n[2]: %s\n[3]: %s\n\n",
				argc, argv[0], argv[1], argv[2], argv[3]);

	libname = argv[1];
	funcname = argv[2];
	funcarg = argv[3];

	hdl = dlopen(libname, RTLD_LAZY);
	if (!hdl) {
		perror("dlopen() failed");
		return EXIT_FAILURE;
	}
	secret_func = (int (*)(int))dlsym(hdl, funcname);
	if (!secret_func) {
		perror("can't find the secret function");
		dlclose(hdl);
		return EXIT_FAILURE;
	}

	printf("%d\n", secret_func(atoi(funcarg)));
	dlclose(hdl);

	return EXIT_SUCCESS;
}