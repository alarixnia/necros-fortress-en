#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/*
 * Format the dialogue into lowercase, etc.
 * Public domain.
 */

#define ROM_SIZE	(8 << 16)
#define DIALOGUE_START	(0x211e0)
#define DIALOGUE_END	(0x2bf50)

static const char *names[] = {
	"I'M",
	"I'VE",
	"I'LL",
	"NECROS",
	"TANKIRIE",
	"TANKIRIE CASTLE",
	"FIPONA FOREST",
	"SIR",
	"LORD",
	"MARIN",
	"RIOAU",
	"SYLVIA",
	"HEBRAIL",
	"MERCENARY",
	"MAGE",
	"AMAZON",
	"KNIGHT",
	"DWARF",
	"ELF",
	"SAMURAI",
	"BERSERKER",
	"RIO",
	"CID",
	"SHOGUN",
	"MECHAROS",
	"MK2",
	"ONIROS",
	"NIROS",
	"JOROS",
	"MAJOROS",
	"SEER",
	"TAKOROS",
	"HEBIROS",
	"DOCROS",
	"GENKAI",
	"BLAZE",
	"ROY",
	"WEST VILLAGE",
	"EAST VILLAGE",
	"DAN",
	"ANGELA",
	"HOPE",
	"NEW HOPE",
	"MOUNT",
	"XENO",
	"HARRISON",
	"KING",
	"PRINCE",
	"ALEPH",
	"CALDERA",
	"BLIZZARD",
	"SYLPHY",
	"MARK",
	"ATLANTIS",
	"ARAO",
	"BLUEGRASS ISLE",
	"FRONTIER ISLAND",
	"DR BOMB",
	"DR. BOMB",
	"THIEF",
	"BARD",
	"NECROGAR",
	"CAPTAIN AIV",
	"CAPT. AIV",
	"CAPT AIV",
};

int
main(int argc, char *argv[])
{
	FILE *origf, *newf;
	uint8_t *b1, *b2;
	size_t ret, offset;
	size_t i, j;
	bool upper = true;

	if (argc < 3) {
		fprintf(stderr,
		    "%s: usage %s original.pce modified.pce\n",
		    argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	b1 = malloc(ROM_SIZE);
	if (b1 == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	b2 = malloc(ROM_SIZE);
	if (b2 == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	origf = fopen(argv[1], "rb");
	if (origf == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	offset = 0;
	for (;;) {
		ret = fread(b1 + offset, 1, 256, origf);
		if (ret == 0) {
			break;
		}
		offset += ret;
	}

	memcpy(b2, b1, ROM_SIZE);

	for (i = DIALOGUE_START; i < DIALOGUE_END; ++i) {
		if (!isascii(b1[i])) {
			if (b1[i] == 0x0f || b1[i] == 0x04 ||
			    b1[i] == 0x05 || b1[i] == 0xeb) {
				upper = true;
			}
			continue;
		}

		if (b1[i] == '?' || b1[i] == '!' || b1[i] == '.') {
			upper = true;
			continue;
		}

		if (b1[i] >= 'A' && b1[i] <= 'Z') {
			b2[i] = upper ? toupper(b1[i]) : tolower(b1[i]);
			upper = false;
		}
	}

	for (i = 0; i < (sizeof(names) / sizeof(char *)); ++i) {
		uint8_t *tmp = b1 + DIALOGUE_START;
		uint8_t *loc;

		while ((loc = strcasestr(tmp, names[i])) != NULL) {
			size_t off = loc - b1;
			size_t max = off + strlen(names[i]);

			if (off > DIALOGUE_END) {
				/* outside dialogue range */
				break;
			}

			/* check we're not in the middle of a word */
			if ((off > 0 && isalpha(b2[off - 1])) ||
			    (max < ROM_SIZE && isalpha(b2[max]))) {
				goto skip;
			}

			b2[off] = toupper(b2[off]);
			for (j = off; j < max; ++j) {
				if (isspace(b2[j]) || b2[j] == 0x02) {
					b2[j + 1] = toupper(b2[j + 1]);
				}
			}

skip:
			tmp = loc + 1;
		}
	}

	/* find and fix "I"s */
	for (i = DIALOGUE_START; i < DIALOGUE_END; ++i) {
		if (b1[i] == 'I' && 
		    (b1[i - 1] < 'A' || b1[i - 1] > 'Z') &&
		    (b1[i + 1] < 'A' || b1[i + 1] > 'Z')) {
			b2[i] = 'I';
		}
	}

	newf = fopen(argv[2], "wb");
	if (newf == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	offset = 0;
	while (offset < ROM_SIZE) {
		size_t n = 256;

		if (n > (ROM_SIZE - offset)) {
			n = ROM_SIZE - offset;
		}

		ret = fwrite(b2 + offset, 1, n, newf);
		if (ret == 0) {
			break;
		}

		offset += ret;
	}

	fclose(origf);
	fclose(newf);
	return EXIT_SUCCESS;
}
