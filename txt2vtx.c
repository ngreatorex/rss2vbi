#include <stdio.h>
#include <stdbool.h>

#define FILENAME "/home/pi/rss2vbi/txt2vtx.c"

int main(int argc, char **argv)
{
	FILE *fh;
	int i, j;
	char c;
	bool eof = false;

	fh = fopen(FILENAME, "r");
	
	while(1) {
		for(j = 0; j < 24; j++) {
			if(j > 0) {
				for(i = 0; i < 40; i++) {
					c = fgetc(fh);
					if((c == EOF) || (c == 255)) eof = true;
					if((eof != true) && (c != '\n')) {
						putchar(c);
					} else {
						while(i < 40) {
							putchar(' ');
							i++;
						}
						break;
					}
				}
			} else {
				printf("              TXT2VTX 000 jan01 00:00/00");
			}
		}
		
		if(eof == true) {
			fclose(fh);
			eof = false;
			fh = fopen(FILENAME, "r");
		}
	}
}
