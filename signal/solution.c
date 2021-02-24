#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

static void sigusr_handler(int signum)
{
	static int sigusr1_cnt = 0;
	static int sigusr2_cnt = 0;

	if (signum == SIGUSR1)
		++sigusr1_cnt;
	if (signum == SIGUSR2)
		++sigusr2_cnt;
	if (signum == SIGTERM) {
		printf("%d %d\n", sigusr1_cnt, sigusr2_cnt);
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	signal(SIGUSR1, sigusr_handler);
	signal(SIGUSR2, sigusr_handler);
	signal(SIGTERM, sigusr_handler);

	while(true);

	return EXIT_SUCCESS;
}

