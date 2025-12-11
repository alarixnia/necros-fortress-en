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

static const char *names[] = {
	"I'M",
	"I'VE",
	"I'LL",
	"NECROS",
	"TANKIRIE CASTLE",
	"TANKIRIE",
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
	"MAJOROS",
	"JOROS",
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

	newf = fopen(argv[2], "wb");
	if (newf == NULL) {
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	offset = 0;

	for (;;) {
		bool upper = true;
		size_t i, j;

		ret = fread(b1, 1, 256, origf);
		if (ret == 0) {
			break;
		}

		for (i = 0; i < ret; ++i) {
			b2[i] = b1[i];

			if ((offset + i) < 0x211e0 ||
			    (offset + i) > 0x2bf50) {
				/* outside dialogue range */
				continue;
			}

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

			if (isalpha(b1[i])) {
				b2[i] = upper ? toupper(b1[i]) : tolower(b1[i]);
				upper = false;
			}
		}

		for (i = 0; i < (sizeof(names) / sizeof(char *)); ++i) {
			uint8_t *tmp = b1;
			uint8_t *loc;

			while ((loc = strstr(tmp, names[i])) != NULL) {
				size_t off = loc - b1;
				size_t max = off + strlen(names[i]);

				/* check we're not in the middle of a word */
				if ((off > 0 && isalpha(b2[off - 1])) ||
				    isalpha(b2[max])) {
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

		ret = fwrite(b2, 1, 256, newf);
		if (ret == 0) {
			break;
		}

		offset += ret;
	}

	fclose(origf);
	fclose(newf);
	return EXIT_SUCCESS;
}
