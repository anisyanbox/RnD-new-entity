#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#define DBG_WORD	"[test]: "

int main(int argc, char const *argv[])
{
	FILE *fp;
	char *line;
	size_t len;
	ssize_t nread;
	char *read_buf;
	int read_buf_cnt;
	int res;
	long int offset;
	int origin;

	char temp_buf[16];
	int temp_var;

	fp = fopen("/dev/solution_node", "r+");
	if (!fp) {
		printf(DBG_WORD "fopen() error\n");
		exit(EXIT_FAILURE);
	}

	/* disable bufferesation */
	setbuf(fp, NULL);

	while ((nread = getline(&line, &len, stdin)) != -1) {
		char c = line[0];

		switch (c) {
		case 'w': /* w<data-to-write-to-device>: w123456789 */
			res = fwrite(line + 1, sizeof(char), nread - 1, fp);
			fflush(fp);
			printf(DBG_WORD "write %ld byte(s) to dev\n", nread - 1);
			printf(DBG_WORD "write(real): %d byte(s)\n", res);
			break;
		case 'r': /* r<cnt-byte-to-read>: r5 */
			read_buf_cnt = atoi(line + 1);
			read_buf = calloc((sizeof(char) * read_buf_cnt) + 1, sizeof(char));

			if (!read_buf) {
				printf(DBG_WORD "malloc() error\n");
				exit(EXIT_FAILURE);
			}

			printf(DBG_WORD "read %d byte(s) from dev: ", read_buf_cnt);
			res = fread(read_buf, sizeof(char), read_buf_cnt, fp);

			*(read_buf + read_buf_cnt) = '\0';
			printf("%s\n", read_buf);
			printf(DBG_WORD "real read: %d byte(s)\n", res);
			free(read_buf);
			break;
		case 'l': /* l<OriginOffset> l0255 - set 255 from start of file */
			origin = *(line + 1);

			if (origin == '0')
				origin = SEEK_SET;
			if (origin == '1')
				origin = SEEK_CUR;
			if (origin == '2')
				origin = SEEK_END;

			offset = atoi(line + 2);
			fseek(fp, offset, origin);
			break;
		default:
			break;
		}
	}

	free(line);
	fclose(fp);
	exit(EXIT_SUCCESS);
}