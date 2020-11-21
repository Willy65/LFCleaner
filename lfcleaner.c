/***************************************************************
 * Tool om LF karakters midden in de regel te verwijderen
 * (c) 2020 - Willy,
 * Basisvariant voor WBS records.
 * Soms wordt midden in de omschrijving een LineFeed geplaatst
 * die proberen we te vinden.
 * Als er achter de LF de kreet "pattern" staat, is het een echte
 * LF, anders is het een foute LF en moeten we die weghalen
 ***************************************************************/
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>

#define SEARCH "wbs_"		// De string waar we naar zoeken
#define LF	0x0a

void print_help(void) {
	printf("This tool replaces LineFeeds (0x0A)\n");
	printf("if they are not followed by a pattern, e.g. \"wbs_\"\n");
	printf("where \"wbs_\" indicates the beginning of the next record\n\n");
	printf("Usage: LineFeedCleaner <filename> <pattern>\n\n");
	printf("Example: LineFeedCleaner plfwbs_20201112_085640.csv wbs_\n");
}

int get_filesize(const char *filename) {
	struct stat st;
	if (stat(filename, &st) == 0) {
		return st.st_size;
	}
	else {
		return -1;
	}
}

int process_data(char *src_stream, char *dst_stream, char *pattern) {
	int patlen, retcode, linecounter;
	char *hptr;					// Help pointer

	retcode = 0;				// We nemen aan dat er geen fouten in het bronbestand zitten
	linecounter = 0;			// We beginnen met regel 0
	patlen = strlen(pattern);	// We hoeven de lengte van het zoekpatroon maar 1x te bepalen

	while (*src_stream != 0) {						// Als we 0 vinden, is dat einde string / bestand
		if (*src_stream != LF) {					// Als het karakter <> LF
			*dst_stream++ = *src_stream++;			// dan het karakter kopieeren
		}
		else {										// Anders... LF gevonden, dan gaan we testen
			linecounter++;							// Regelteller ophogen
			hptr = src_stream + 1;					// Hulp pointer wijst naar eerstvolgende karakter
			if (*hptr == 0) {						// Hulp pointer is 0: einde bestand gevonden
				*dst_stream++ = *src_stream++;		// einde bestand: Laatste LF kopieeren
			}
			else {									// Hulp pointer <> 0, dan kijken naar de volgende regel
				if (*hptr == LF) {					// Volgende regel begint ook met LF?
					fprintf(stderr, "Extra linefeed found at line %i\n", linecounter);
					src_stream++;					// src_stream ophogen en de extra LF overslaan
				}
				else {								// *src_stream == LF && *hptr <> LF
					if (strncmp(hptr, pattern, patlen) == 0) {	// Start de volgende regel met pattern ?
						*dst_stream++ = *src_stream++;	// Ja, de volgende regel begint met wbs_, dus LF kopieeren
					}
					else {								// Nee, de volgende regel begint niet met pattern,
														// dan hebben we een bogus linefeed gevonden.
						retcode = 1;					// Aangeven dat we een fout hebben gevonden
						src_stream++;					// niet kopieeren, wel src_stream ophogen
						fprintf(stderr, "Bad Linefeed found at line %i\n", linecounter);
					}
				}
			}
		}
	}
	*dst_stream = 0;	// Zeker maken dat dst_stream een 0 terminator heeft
	return retcode;
}

int main(int argc, char *argv[]) {
	FILE *fp;
	int filesize;
	char *data_in, *data_out;
	int result;

	// Eerst controleren of we 2 argumenten hebben: Bestandsnaam en zoekpatroon
	if(argc != 3) {				// Check of er 2 argumenten zijn na de programmanaam
		print_help();			// En anders help afdrukken
		return EXIT_FAILURE;	// Einde oefening
	}

	filesize = get_filesize(argv[1]);	// Grootte van de file opvragen
	if (filesize == -1) {				// Bestand kan niet worden geopend, weer einde oefening
		fprintf(stderr, "Error: Cannot open file\n");	// Bestand kan niet geopend / gevonden worden
		return EXIT_FAILURE;			// Bestand kan niet geopend worden, einde oefening.
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
