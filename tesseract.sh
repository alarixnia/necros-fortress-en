#!/bin/sh
tesseract "$*" - --oem 1 --psm 6 -l jpn | sed -e 's, ,,g' | \
xclip -selection CLIPBOARD
