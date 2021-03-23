#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sigusr_handler(int signum)
{
	if (signum == SIGURG)
		exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = fork();
	if (pid == -1) {
		fprintf(stderr, "fork() failed %s", strerror(errno));
		return EXIT_FAILURE;
	} else if (pid != 0) {
		/*
                 * I'm a parent.
                 * It's needed to exit from parent's process, because we want to crate a deamon
                 * and it's needed child was from init process (with ppid = 1).
                 */
 		exit(EXIT_SUCCESS);
	} else {
		/* child */
		printf("%d\n", (int)getpid());

		/* add signal to finish the daemon */
		signal(SIGURG, sigusr_handler);

		/* this child will be a group leader in a new process's group */
		if (setsid() == -1) {
			fprintf(stderr, "setsid() failed %s", strerror(errno));
			return EXIT_FAILURE;
		}

		if (chdir("/") == -1) {
			fprintf(stderr, "chdir() failed %s", strerror(errno));
			return EXIT_FAILURE;
		}

		fclose(stdin);
		fclose(stdout);
		fclose(stderr);

		while (true) {
			usleep(500000);
		}
	}

	return EXIT_SUCCESS;
}

