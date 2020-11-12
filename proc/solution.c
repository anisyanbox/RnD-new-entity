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

#define NEEDLE_STR	"PPid:\t"
#define NEEDLE_STR_LEN	(sizeof(NEEDLE_STR) - 1)

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

/* Returns -1 if str doesn't contain PPID. 
 * Othervise returns PPID. */
static char *find_ppid(char *str)
{
	int i;
	char *search;
	static char ppid_str[32];

	search = strstr((const char *)str, NEEDLE_STR);
	if (!search)
		return NULL;

	for (i = 0; i < 32; ++i) {
		ppid_str[i] = str[NEEDLE_STR_LEN + i];
		if (ppid_str[i] == '\n') {
			ppid_str[i] = '\0';
			break;
		}
	}

	return ppid_str;
}

int main(int argc, char *argv[])
{
	int n;
	FILE *fp;
	pid_t pid;
	char fname[256] = { 0 };
	char *lineptr;
	int line_cnt;
	size_t line_size;
	ssize_t ret;
	char *ppid;
	bool break_cond = false;

	/* Current process's PID */
	pid = getpid();
	debug_out("cur pid: %d\n", (int)pid);

	/* create file name */
	n = snprintf(fname, 256, "/proc/%d/status", (int)pid);
	if (n > 256) {
		fprintf(stderr, "snprintf out of memory: %s\n",
				strerror(errno));
		return EXIT_FAILURE;
	}

	debug_out("file name: %s\n", fname);

	fp= fopen((const char *)fname, "r");
	if (!fp)  {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	line_cnt = 0;

	while (!break_cond) {
		lineptr = NULL;
		line_size = 0;

		ret = getline(&lineptr, &line_size, fp);
		if (ret == -1) {
			if  (errno == EINVAL || errno == ENOMEM) {
				fprintf(stderr, "getpid failed: %s\n",
					strerror(errno));
				free(lineptr);
				return EXIT_FAILURE;
			}
			
			/* EOF --> break the cycle*/
			break_cond = true;
		}

		debug_out("Line %d: \t\t%s", ++line_cnt, lineptr);

		ppid = find_ppid(lineptr);
		if (ppid != NULL)
			break_cond = true;	

		free(lineptr);
	}

	printf("%s\n", ppid);

	return 0;
}

