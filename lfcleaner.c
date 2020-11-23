/********************************************************************
 * Tool to remove extra / stray linefeeds from a CSV file
 * (c) 2020, Willem Oldeman - Netherlands
 *
 * Often, CSV (comma seperated values) files contain a comment field,
 * but users add linefeeds to this comment field.
 * Also, every line in the CSV file begins with <pattern>
 *
 * This gives us a way to remove extra linefeeds, if it isn't
 * followed by <pattern> or if it is <end of file - EOF>
 ***************************************************************/
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>

#define LF	0x0a	// To be replaced by a future release
					// where it can be passed as an argument

void print_help(void) {
	// If too few or too many arguments are given, print help
	printf("This tool replaces LineFeeds (0x0A)\n");
	printf("if they are not followed by a pattern, e.g. \"wbs_\"\n");
	printf("where \"wbs_\" indicates the beginning of the next record\n\n");
	printf("Usage: LineFeedCleaner <filename> <pattern>\n\n");
	printf("Example: LineFeedCleaner plfwbs_20201112_085640.csv wbs_\n");
}

int get_filesize(const char *filename) {
	// Get the size of the input file.
	// Return -1 in case of error
	struct stat st;
	if (stat(filename, &st) == 0) {
		return st.st_size;
	}
	else {
		return -1;
	}
}

int process_data(char *src_stream, char *dst_stream, char *pattern) {
	// Here the processing takes place
	// LF before end of file is accepted, LF before <pattern> is accepted.
	// If not, the LF is discarded.
	int patlen, retcode, linecounter;
	char *hptr;					// Help pointer

	retcode = 0;				// We assume that there are no errors in the input file
	linecounter = 0;			// Start at line 0
	patlen = strlen(pattern);	// We only need to find the length once of search pattern

	while (*src_stream != 0) {						// If source stream = 0, it means end of file
		if (*src_stream != LF) {					// If character <> LF
			*dst_stream++ = *src_stream++;			// then copy it
		}
		else {										// Else... LF found, so test for EOF or <pattern>
			linecounter++;							// Increment linecounter (used for error logging)
			hptr = src_stream + 1;					// Help pointer points to the next character
			if (*hptr == 0) {						// Help pointer is 0: Found EOF (end of file)
				*dst_stream++ = *src_stream++;		// EOF: copy the last LF
			}
			else {									// Help pointer <> 0, then look at the next line
				if (*hptr == LF) {					// Does it start with LF?
					fprintf(stderr, "Extra linefeed found at line %i\n", linecounter);	// Yes, it does
					src_stream++;					// Increment src_stream and skip the extra LF
				}
				else {								// *src_stream == LF && *hptr <> LF
					if (strncmp(hptr, pattern, patlen) == 0) {	// Does the next line start with pattern ?
						*dst_stream++ = *src_stream++;	// Yes, the next line starts with <pattern>, so we copy the LF
					}
					else {								// No, the next line doesn't start with <pattern>
														// If so, we found a bogus linefeed
						retcode = 1;					// Indicate that an error was found
						src_stream++;					// Don't copy the LF, but do increment src_stream
						fprintf(stderr, "Bad Linefeed found at line %i\n", linecounter);
					}
				}		// if (*hptr == LF) else
			}			// if (*hptr == LF)
		}				// while (*src_stream != 0) else
	}					// while (*src_stream != 0)
	*dst_stream = 0;	// Zeker maken dat dst_stream een 0 terminator heeft
	return retcode;
}

int main(int argc, char *argv[]) {
	FILE *fp;
	int filesize;
	char *data_in, *data_out;
	int result;

	// First check if we have 2 arguments: Filename and search pattern
	if(argc != 3) {				// Check if there are 2 arguments after <programname>
		print_help();			// If not, print help
		return EXIT_FAILURE;	// Exit program
	}

	filesize = get_filesize(argv[1]);	// Get filesize
	if (filesize == -1) {				// File can't be opened, end.
		fprintf(stderr, "Error: Cannot open file\n");	// File can't be opened, exit with error.
		return EXIT_FAILURE;
	}

	data_in = (char *) malloc(filesize + 1);	// Geheugen reserveren om de file in te lezen + \0 terminator
	if (data_in == NULL) {						// Kunnen we geheugen krijgen?
		fprintf(stderr, "Error: Memory allocation error for input\n");
		return EXIT_FAILURE;					// Geen geheugen, einde oefening.
	}

	data_out = (char *) malloc(filesize + 1);	// Geheugen reserveren om resultaat in weg te schrijven
	if (data_out == NULL) {						// Kunnen we geheugen krijgen?
		fprintf(stderr, "Error: Memory allocation error for output\n");
		return EXIT_FAILURE;					// Geen geheugen, einde oefening.
	}

	fp = fopen(argv[1], "rb");					// File openen om te lezen
	if (fp != NULL) {
		fread(data_in, sizeof(char), filesize, fp);	// In 1 keer in het geheugen inlezen
		data_in[filesize - 1] = 0;					// String afsluiten
		fclose(fp);									// Input sluiten

		result = process_data(data_in, data_out, argv[2]);	// Deze functie zoekt Linefeeds en verwijdert ze indien nodig

		printf("%s\n", data_out);
	}

	free(data_in);								// Geheugen teruggeven, input buffer
	free(data_out);								// Geheugen teruggeven, output buffer

	return result;
}
