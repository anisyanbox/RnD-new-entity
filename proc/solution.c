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
	
	if (argc != 2) {
		return 0;
	} else {
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
#define PPID_STR	"PPid:\t"
#define PPID_STR_LEN	(sizeof(PPID_STR) - 1)

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

		search_str = strstr((const char *)lineptr, PPID_STR);
		if (!search_str)
			continue;

		/* replace EOL to '\0' */
		for (i = 0; i < 32; ++i) {
			ppid_str[i] = lineptr[PPID_STR_LEN + i];
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

static int proc_dirs_selector(const struct dirent *d)
{
	int i;
	int c;

	for (i = 0; i < 256; ++i) {
		c = d->d_name[i];

		if (c == '\0')
			break;

		if (!isdigit(c))
			return 0;
	}

	return 1;
}

static int get_proc_dirs(struct dirent ***namelist)
{
	int n;

	n = scandir("/proc", namelist, proc_dirs_selector, alphasort);
	if (n == -1) {
		fprintf(stderr, "scandir() failed: %s\n", strerror(errno));
		*namelist = NULL;
	}

	return n;
}

/*
 * Recursice function to find all child processes for known pid.
 *
 * @pid		- parent pid
 * @proc_list	- list of all available processes's dirs in system
 * @proc_cnt	- count of items in proclist
 *
 * Returns count of children.
 */
static int get_all_child_for_pid(pid_t pid, struct dirent **proc_list,
	int proc_cnt)
{
	int i, n;
	int children = 0;

	pid_t ppid;
	pid_t temp_pid;

	for (i = 0; i < proc_cnt; ++i) {
		temp_pid = atoi((*proc_list[i]).d_name);
		ppid = get_ppid(temp_pid);

		/* ppid for some proc == input PID --> it means
		 that this */
		if (ppid == pid) {
			++children;
			children += get_all_child_for_pid(temp_pid,
				proc_list, proc_cnt);
		}
	}

	return children;
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int i, n;
	struct dirent **namelist;

	pid = (pid_t)check_input(argc, argv);
	if (pid == 0) {
		fprintf(stderr, "wrong input args\n");
		return EXIT_FAILURE;
	}

	n = get_proc_dirs(&namelist);
	if (!namelist) {
		fprintf(stderr, "get right proc dirs failed\n");
		return EXIT_FAILURE;
	}

	printf("%d\n", get_all_child_for_pid(pid, namelist, n) + 1);

	while (n--)
		free(namelist[n]);
	free(namelist);

	return EXIT_SUCCESS;
}

