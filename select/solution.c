#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

/* returns -1 if EOF */
int do_read(int fd)
{
	int n = 0;
	char buf[64] = { 0 };

	n = read(fd, buf, 64);
	if (n < 0) {
		errx(1, "read fd=%d failed", fd);
	}

	if (n == 0)
		return -1;

	return atoi(buf);
}

int main(int argc, char *argv[])
{
	int f1;
	int f2;
	int res, tmp;
	int sum = 0;
	int read_done1 = 0;
	int read_done2 = 0;
	fd_set rfds;

	f1 = open("./in1", O_RDONLY | O_NONBLOCK);
	f2 = open("./in2", O_RDONLY | O_NONBLOCK);

	if (f1 < 0 || f2 < 0) {
		fprintf(stderr, "open error\n");
		return EXIT_FAILURE;
	}

	while (true) {
		FD_ZERO(&rfds);
		
		if (!read_done1)
			FD_SET(f1, &rfds);
		if (!read_done2)
			FD_SET(f2, &rfds);

		res = select(f2 + 1, &rfds, NULL, NULL, NULL);
		if (res == - 1) {
			perror("select error\n");
			return EXIT_FAILURE;
		}

		if (FD_ISSET(f1, &rfds)) { /* check in1 */
			tmp = do_read(f1);
			if (tmp < 0)
				read_done1 = 1;
			 else
				sum += tmp;
		}
		if (FD_ISSET(f2, &rfds)) { /* check in2 */
			tmp = do_read(f2);
			if (tmp < 0)
                                read_done2 = 1;
			else
				sum += tmp;
		}

		/* break the cycle if both pipes were read */
		if (read_done1 && read_done2)
			break;
	}

	close(f1);
	close(f2);

	printf("%d\n", sum);

	return EXIT_SUCCESS;
}

