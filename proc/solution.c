#include <dirent.h>
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

#define NEEDLE_STR	"Name:\tgenenv\n"
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

static int selector(const struct dirent *d)
{
	FILE *fp;
	int n;
	char fname[256] = { 0 };
	bool break_cond = false;
	char *lineptr;
	size_t line_size;
	ssize_t ret;
	int ret2 = 0;

	/* create file name */
	n = snprintf(fname, 256, "/proc/%s/status", d->d_name);
	if (n > 256) {
		/* fprintf(stderr, "snprintf out of memory: %s\n",
				strerror(errno)); */
		return ret2;
	}

	fp= fopen((const char *)fname, "r");
	if (!fp)  {
		/* fprintf(stderr, "Error opening file: %s\n", strerror(errno)); */
		return ret2;
	}

	while (!break_cond) {
		lineptr = NULL;
		line_size = 0;

		ret = getline(&lineptr, &line_size, fp);
		if (ret == -1) {
			if  (errno == EINVAL || errno == ENOMEM) {
				/* fprintf(stderr, "getpid failed: %s\n",
					strerror(errno)); */
				goto out;
			}
			
			/* EOF --> break the cycle*/
			break_cond = true;
		}

		/* find NEEDLE_STR */
		if (strcmp(lineptr, NEEDLE_STR) == 0) {
			ret2 = 1;
			goto out;
		}
	}

out:
	free(lineptr);
	fclose(fp);

	return ret2;
}

int main(int argc, char *argv[])
{
	int n, proc_cnt;
	struct dirent **namelist;

	n = scandir("/proc", &namelist, selector, alphasort);
	if (n < 0) {
		fprintf(stderr, "scandir() failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	proc_cnt = n;
	while (n--)
		free(namelist[n]);
	free(namelist);

	printf("%d\n", proc_cnt);

	return EXIT_SUCCESS;
}

