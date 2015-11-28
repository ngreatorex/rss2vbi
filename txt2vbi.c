/* (c) 2015 Chris Young
 * http://www.unsatisfactorysoftware.co.uk
 */
 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#include "vbi.h"

#define START_MAG  1
#define START_TEN  0
#define START_PAGE 0

static void get_hostname(char *buf)
{
	char tmp[11];
	char *dot;

	gethostname(tmp, 11);
	dot = strchr(tmp, '.');
	tmp[dot - tmp] = 0;
	
	sprintf(buf, "\x06%s", tmp);
}

static void get_time(char *buf)
{
	/* this function is borrowed from http://github.com/ali1234/raspi-teletext */
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(buf, 21, "\x03%a %d %b\x02%H:%M/%S", info);
}

int main(int argc, char **argv)
{
	FILE *fh;
	int i, j;
	char c;
	bool eof = false;
	int mag = START_MAG, ten = START_TEN, page = START_PAGE;
	char *buffer = malloc(42);

	if(argc != 2) {
		printf("%s <filename>\n\n", argv[0]);
		return 5;
	}
	
	fh = fopen(argv[1], "r");
	
	while(1) {
		for(j = 0; j < 24; j++) {
			if(j > 0) {
				for(i = 0; i < 40; i++) {
					c = fgetc(fh);
					if((c == EOF) || (c == (char)255)) eof = true;
					if((eof != true) && (c != '\n')) {
						buffer[i+2] = c;
					} else {
						while(i < 40) {
							buffer[i+2] = ' ';
							i++;
						}
						break;
					}
				}
			} else {
				get_hostname(buffer + 10); //8
				sprintf(buffer + 18, "\x06%d%d%d", mag, ten, page);
				get_time(buffer + 22); /* injects date/time */
			}

			pkt_header(buffer, j, mag, ten, page);
			
			for(i = 0; i < 42; i++) {
				putchar(buffer[i]);
			}
		}
		
		page++;
		if(page == 10) {
			page = 0;
			ten++;
			
			if(ten == 10) {
				ten = 0;
				mag++;
			}
		}
		
		if(eof == true) {
			fclose(fh);
			eof = false;
			mag = START_MAG;
			ten = START_TEN;
			page = START_PAGE;
			fh = fopen(argv[1], "r");
		}
	}
	
	free(buffer);
	return 0;
}
