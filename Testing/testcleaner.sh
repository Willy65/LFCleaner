#!/bin/bash
# Testscript for LineFeedCleaner.
# Point PROGDIR to where LineFeedCleaner is installed
#

PROGDIR="../Release"
PROGNAME="LineFeedCleaner"

for FILE in \
	"demo_perfect.txt" \
	"demo_extra_lfs.txt" \
	"demo_broken_lf.txt" \
	"demo_linefeed.txt" \
	"demo_double_bad_lf.txt"
do
	clear
	echo $FILE
	cat $FILE
	echo
	$PROGDIR/$PROGNAME $FILE "pattern" > tmp
	RESULT=$?
	echo
	echo "Outputfile ="
	cat tmp
	echo
	echo "Diff ="
	diff $FILE tmp
	echo
	NUMLIN1=$(cat $FILE | wc -l)
	NUMLIN2=$(cat tmp   | wc -l)
	echo "Numlines original: $NUMLIN1"
	echo "Numlines result  : $NUMLIN2"
	echo "Result           : $RESULT"
	printf "\nPress enter for next file"
	read A
done
rm -f tmp

