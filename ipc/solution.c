#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

static key_t get_unique_key(void)
{
	char path[256];

	sprintf(path, "/proc/%d/status", getpid());

	return ftok(path, '0');
}

int main(int argc, char *argv[])
{
	key_t k1 = 0;
	key_t k2 = 0;
	key_t k3 = 0;

	void *buf1 = NULL;
	void *buf2 = NULL;
	void *buf3 = NULL;

	int shmid1 = 0;
	int shmid2 = 0;
	int shmid3 = 0;

	char err[256];

	if (argc < 3 || !argv || !argv[1] || !argv[2]) {
		fprintf(stderr, "bad args\n");
		return EXIT_FAILURE;
	}

	/* keys of existing shm objects */
	k1 = (key_t)atoi(argv[1]);
	k2 = (key_t)atoi(argv[2]);

	/*
 	 * Attach the shm objs to this process
 	 */
	shmid1 = shmget(k1, 1000, 0666);
	if (shmid1 == -1) {
		sprintf(err,  "can't get an identifier for key=%d", k1);
		perror(err);
		return EXIT_FAILURE;
	}

	shmid2 = shmget(k2, 1000, 0666);
        if (shmid2 == -1) {
		sprintf(err,  "can't get an identifier for key=%d", k2);
		perror(err);
		return EXIT_FAILURE;
	}
 
	buf1 = shmat(shmid1, NULL, SHM_RDONLY);
	if (buf1 == (void *)-1) {
		sprintf(err,  "can't attach object with=%d", shmid1);
		perror(err);
		return EXIT_FAILURE;
	}

	buf2 = shmat(shmid2, NULL, SHM_RDONLY);
        if (buf2 == (void *)-1) {
		sprintf(err,  "can't attach object with=%d", shmid2);
		perror(err);
		return EXIT_FAILURE;
	}

	/*
 	 * Create own shm obj
 	 */
	k3 = get_unique_key();
	shmid3 = shmget(k3, 1000, IPC_CREAT | 0666);	
	if (shmid3 == -1) {
		sprintf(err,  "can't get an identifier for own key=%d", k3);
		perror(err);
		return EXIT_FAILURE;
	}

	buf3 = shmat(shmid3, NULL, 0);
	if (buf3 == (void *)-1) {
		sprintf(err,  "can't attach my object with=%d", shmid3);
		perror(err);
		return EXIT_FAILURE;
	}

	/*
 	 * Summ buf1 and buf2 and save the result to buf3
 	 */
	int i;
	int *b1 = (int *)buf1;
	int *b2 = (int *)buf2;
	int *b3 = (int *)buf3;

	for (i = 0; i < 100; ++i) {
		b3[i] = b1[i] + b2[i];
	}

	printf("%d\n", k3);

	return EXIT_SUCCESS;
}

