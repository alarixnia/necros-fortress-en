#include <stdio.h>
#include <stdlib.h>

char *charset[256];

void *
read_file_full(const char *path, size_t *len)
{
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t *buffer = malloc(length);
	if (buffer == NULL) {
		fclose(file);
		return NULL;
	}

	fread(buffer, 1, length, file);
	fclose(file);

	if (len != NULL) {
		*len = (size_t)length;
	}

	return buffer;
}

int
main(int argc, char **argv)
{
	charset[0x04] = "ー";
	charset[0x10] = "AMAZON";
	charset[0x11] = "ELF";
	charset[0x12] = "SAMURAI";
	charset[0x13] = "DWARF";
	charset[0x14] = "KNIGHT";
	charset[0x15] = "BERSERKER";
	charset[0x16] = "MERCENARY";
	charset[0x17] = "MAGE";
	charset[0x20] = " ";
	charset[0x21] = "!";
	charset[0x22] = "\"";
	charset[0x27] = "'";
	charset[0x28] = "(";
	charset[0x29] = ")";
	charset[0x2a] = "*";
	charset[0x2b] = "+";
	charset[0x2c] = ",";
	charset[0x2d] = "-";
	charset[0x2e] = ".";
	charset[0x3f] = "?";
	charset[0x66] = "を";
	charset[0x6c] = "ゃ";
	charset[0x6d] = "ゅ";
	charset[0x6e] = "ょ";
	charset[0x6f] = "っ";
	charset[0x71] = "あ";
	charset[0x72] = "い";
	charset[0x73] = "う";
	charset[0x74] = "え";
	charset[0x75] = "お";
	charset[0x76] = "か";
	charset[0x77] = "き";
	charset[0x78] = "く";
	charset[0x79] = "け";
	charset[0x7a] = "こ";
	charset[0x7b] = "さ";
	charset[0x7c] = "し";
	charset[0x7d] = "す";
	charset[0x7e] = "せ";
	charset[0x7f] = "そ";
	charset[0x80] = "た";
	charset[0x81] = "ち";
	charset[0x82] = "つ";
	charset[0x83] = "て";
	charset[0x84] = "と";
	charset[0x85] = "な";
	charset[0x86] = "に";
	charset[0x87] = "ぬ";
	charset[0x88] = "ね";
	charset[0x89] = "の";
	charset[0x8a] = "は";
	charset[0x8b] = "ひ";
	charset[0x8c] = "ふ";
	charset[0x8d] = "へ";
	charset[0x8e] = "ほ";
	charset[0x8f] = "ま";
	charset[0x90] = "み";
	charset[0x91] = "む";
	charset[0x92] = "め";
	charset[0x93] = "も";
	charset[0x94] = "や";
	charset[0x95] = "ゆ";
	charset[0x96] = "よ";
	charset[0x97] = "ら";
	charset[0x98] = "り";
	charset[0x99] = "る";
	charset[0x9a] = "れ";
	charset[0x9b] = "ろ";
	charset[0x9c] = "わ";
	charset[0x9d] = "ん";
	charset[0xa1] = "。";
	charset[0xa2] = "「";
	charset[0xa4] = "、";
	charset[0xa5] = "・";
	charset[0xa6] = "ヲ";
	charset[0xa7] = "ァ";
	charset[0xa8] = "ィ";
	charset[0xab] = "ォ";
	charset[0xac] = "ャ";
	charset[0xad] = "ュ";
	charset[0xae] = "ョ";
	charset[0xaf] = "ッ";
	charset[0xb0] = "ー";
	charset[0xb1] = "ア";
	charset[0xb2] = "イ";
	charset[0xb3] = "ウ";
	charset[0xb4] = "エ";
	charset[0xb5] = "オ";
	charset[0xb6] = "カ";
	charset[0xb7] = "キ";
	charset[0xb8] = "ク";
	charset[0xb9] = "ケ";
	charset[0xba] = "コ";
	charset[0xbb] = "サ";
	charset[0xbc] = "シ";
	charset[0xbe] = "セ";
	charset[0xbf] = "ソ";
	charset[0xbd] = "ス";
	charset[0xc0] = "タ";
	charset[0xc1] = "チ";
	charset[0xc2] = "ツ";
	charset[0xc3] = "テ";
	charset[0xc4] = "ト";
	charset[0xc5] = "ナ";
	charset[0xc6] = "ニ";
	charset[0xc7] = "ヌ";
	charset[0xc8] = "ネ";
	charset[0xc9] = "ノ";
	charset[0xca] = "ハ";
	charset[0xcb] = "ヒ";
	charset[0xcc] = "フ";
	charset[0xcd] = "ヘ";
	charset[0xce] = "ホ";
	charset[0xcf] = "マ";
	charset[0xd0] = "ミ";
	charset[0xd1] = "ム";
	charset[0xd2] = "メ";
	charset[0xd3] = "モ";
	charset[0xd4] = "ヤ";
	charset[0xd5] = "ヤ";
	charset[0xd6] = "ヨ";
	charset[0xd7] = "ラ";
	charset[0xd8] = "リ";
	charset[0xd9] = "ル";
	charset[0xdb] = "ロ";
	charset[0xda] = "レ";
	charset[0xdc] = "ワ";
	charset[0xdd] = "ン";
	charset[0xde] = "゙";
	charset[0xdf] = "゚";
	charset[0xeb] = "...";
	charset[0xec] = "~";

	size_t len;
	char *buf = read_file_full(argv[1], &len);
	if (buf == NULL) {
		printf("failed to read\n");
		return 1;
	}
	for (size_t i = 0; i < len; ++i) {
		uint8_t ch = buf[i] & 0xff;
		if (ch == 0x02 || ch == 0x0f) {
			printf("\n");
			continue;
		}
		if (ch == 0x04 || ch == 0x05) {
			printf("\n[...]\n");
			continue;
		}
		if (ch >= '0' && ch <= '9') {
			fputc(ch, stdout);
		} else if (ch >= 'A' && ch <= 'Z') {
			fputc(ch, stdout);
		} else if (charset[ch] != NULL) {
			printf("%s", charset[ch]);
		} else {
			printf("?");
		}
		if (ch == 0xa1 || ch == 0xeb) {
			printf("\n");
		}
	}
	printf("\n");
	return 0;
}
