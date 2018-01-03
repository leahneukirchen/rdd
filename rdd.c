/* rdd - random data dump */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "sosemanuk.h"

#define fail(i, s) write(2, s, (sizeof s)-1), exit(i);

int
main(int argc, char *argv[])
{
	unsigned char key[32], iv[16], buf[120*512];
	sosemanuk_key_context kc;
	sosemanuk_run_context rc;

	int fd, c, v = 0;
	long i, r = 4;
	const char *src = "/dev/urandom";

	while ((c = getopt(argc, argv, "i:r:v")) != -1)
		switch (c) {
		case 'i': src = optarg; break;
		case 'r': r = atoi(optarg); break;
		case 'v': v = 1; break;
		default:
usage:
			fail(1, "Usage: rdd [-i RANDOMSOURCE] [-r REKEYMB] [-v]\n");
		}

	if (argc > optind)
		goto usage;

	if (isatty(1))
		fail(5, "cowardly not dumping to tty\n");

	if ((fd = open(src, O_RDONLY)) < 0)
		fail(2, "failed to open random source\n");

	if (read(fd, key, sizeof key) != sizeof key)
		fail(3, "failed to read key from random source\n");
	sosemanuk_schedule(&kc, key, sizeof key);

	while (1) {
		if (read(fd, iv, sizeof iv) != sizeof iv)
			fail(3, "failed to read iv from random source\n");
		sosemanuk_init(&rc, &kc, iv, sizeof iv);
		if (v)
			write(2, ".", 1);

		for (i = 0; r < 0 || i < r*1024*1024; i += sizeof buf) {
			sosemanuk_prng(&rc, buf, sizeof buf);
			while (write(1, buf, sizeof buf) != sizeof buf)
				if (errno && errno != EINTR)
					fail(4, "write error\n");
		}
	}
}
