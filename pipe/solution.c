#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
	char *cmd;
	size_t len;
	int c, n;
	FILE *fp;

	if (argc != 3 || !argv || !argv[1] || !argv[2]) {
		fprintf(stderr, "wrong input\n");
		return EXIT_FAILURE;
	}

	len = strlen(argv[1]);
	len += strlen(argv[2]);

	/* don't forget space symbol and \0 */
	cmd = malloc(len + 2);
	if (!cmd) {
		fprintf(stderr, "malloc error\n");
		return EXIT_FAILURE;
	}

	n = snprintf(cmd, len + 2, "%s %s", argv[1], argv[2]);
	if (n > len + 2) {
		fprintf(stderr, "snprintf error\n");
		return EXIT_FAILURE;
	}


	fp = popen(cmd, "r");
	if (!fp) {
		fprintf(stderr, "popen error\n");
		return EXIT_FAILURE;
	}

	n = 0;

	while (true) {
		c = fgetc(fp);
		if (c == EOF)
			break;

		if (c == '0')
			++n;
	}

	pclose(fp);
	free(cmd);

	printf("%d\n", n);

	return EXIT_SUCCESS;
}

