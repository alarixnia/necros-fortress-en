#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * Verify that all existing dialogue ending markers are in
 * the same position.
 */

int
main(int argc, char *argv[])
{
	FILE *origf, *newf;
	uint8_t b1[256], b2[256];
	size_t ret, offset;

	if (argc < 3) {
		fprintf(stderr,
		    "%s: usage %s original.pce modified.pce\n",
		    argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	origf = fopen(argv[1], "rb");
	if (origf == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	newf = fopen(argv[2], "rb");
	if (newf == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	offset = 0;
	for (;;) {
		size_t i;

		ret = fread(b1, 1, 256, origf);
		if (ret == 0) {
			break;
		}

		ret = fread(b2, 1, 256, newf);
		if (ret == 0) {
			break;
		}

		for (i = 0; i < 256; ++i) {
			if (b1[i] == 0x0f || b1[i] == 0x04) {
				if (b2[i] != b1[i]) {
					printf("missing marker at: 0x%lx",
					    (offset + i));
					printf(" (was 0x%02x)\n", b1[i]);
				}
			} else if (b2[i] == 0xf || b2[i] == 0x04) {
				printf("stray marker at: 0x%lx\n",
				    (offset + i));
			} else if ((b1[i] > 0x05 && b1[i] < 0x10) ||
			    b1[i] > 0xec || b1[i] == 0x01 ||
			    (b1[i] > 0x05 && b1[i] < 0x0f)) {
				if (b2[i] != b1[i]) {
					printf("progam byte lost at: 0x%lx",
					    (offset + i));
					printf(" (was 0x%02x)\n", b1[i]);
				}
			}
		}

		offset += ret;
	}

	return EXIT_SUCCESS;
}
