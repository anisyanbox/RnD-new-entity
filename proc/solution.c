#include <dirent.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static void debug_out(const char *fmt, ...)
{
#ifdef DEBUG_PROC
	va_list args;

	if (!fmt)
		return;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
#endif
}

static int check_input(int argc, char *argv[])
{
	int i;
	int len;
	char *str;

	if (argc == 2) {
		str = argv[1];
		len = strlen(str);

		for (i = 0; i < len; ++i)
			if (!isdigit(str[i]))
				return 0;
	}

	return atoi(str);
}

static int get_ppid(pid_t pid)
{
#define NEEDLE_STR	"PPid:\t"
#define NEEDLE_STR_LEN	(sizeof(NEEDLE_STR) - 1)

	int i;
	int n;

	FILE *fp;

	char *search_str;
	char ppid_str[32];
	char proc_fname[256];

	bool break_cond = false;

	char *lineptr;
	size_t linesize;
	ssize_t ret;

	/* create process file name */
	n = snprintf(proc_fname, 256, "/proc/%d/status", (int)pid);
	if (n > 256) {
		fprintf(stderr, "snpintf out of memory\n");
		return -1;
	}

	debug_out("proc file name: %s\n", proc_fname);

	fp = fopen(proc_fname, "r");
	if (!fp) {
		fprintf(stderr, "fopen() failed: %s", strerror(errno));
		return -1;
	}

	while (!break_cond) {
		lineptr = NULL;
		linesize = 0;

		ret = getline(&lineptr, &linesize, fp);
		if (ret == -1) {
			if  (errno == EINVAL || errno == ENOMEM) {
				fprintf(stderr, "getline failed: %s\n",
					strerror(errno));
				free(lineptr);
				return -1;
			}

			/* EOF --> break the cycle*/
			break_cond = true;
		}

		search_str = strstr((const char *)lineptr, NEEDLE_STR);
		if (!search_str)
			continue;

		/* replace EOL to '\0' */
		for (i = 0; i < 32; ++i) {
			ppid_str[i] = lineptr[NEEDLE_STR_LEN + i];
			if (ppid_str[i] == '\n') {
				ppid_str[i] = '\0';
				break_cond = true;
				break;
			}
		}

		free(lineptr);
	}

	fclose(fp);

	return atoi(ppid_str);
}

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = (pid_t)check_input(argc, argv);
	if (pid == 0) {
		fprintf(stderr, "wrong input args\n");
		return EXIT_FAILURE;
	}

	printf("%d\n", pid);
	while (true) {
		pid = get_ppid(pid);
		if (pid <= 0)
			return EXIT_FAILURE;

		printf("%d\n", pid);

		if (pid == 1)
			break;
	}

	return EXIT_SUCCESS;
}

