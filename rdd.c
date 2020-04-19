/* rdd - random data dump */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "chacha.h"

#define fail(i, s) write(2, "rdd: " s, 5+(sizeof s)-1), exit(i);

int
main(int argc, char *argv[])
{
	chacha_state state;
	chacha_key key;
	chacha_iv iv;
	unsigned char buf[16*4096];

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

	if (chacha_startup() != 0)
		fail(255, "self-test failed\n");

	if (isatty(1))
		fail(5, "cowardly not dumping random data to tty\n");

	if ((fd = open(src, O_RDONLY)) < 0)
		fail(2, "failed to open random source\n");

	if (read(fd, iv.b, sizeof iv.b) != sizeof iv.b)
		fail(3, "failed to read iv from random source\n");

	while (1) {
		if (read(fd, key.b, sizeof key.b) != sizeof key.b)
			fail(3, "failed to read key from random source\n");
		chacha_init(&state, &key, &iv, 8);

		if (v)
			write(2, ".", 1);

		for (i = 0; r < 0 || i < r*1024*1024; i += sizeof buf) {
			chacha_update(&state, 0, buf, sizeof buf);
			while (write(1, buf, sizeof buf) != sizeof buf)
				if (errno) {
					if (errno == ENOSPC || errno == EPIPE)
						exit(0);
					if (errno != EINTR)
						fail(4, "write error\n");
				}
		}
	}
}
