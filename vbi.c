/* Convert .BIN to VBI */
/* (c) 2015 Chris Young
 * http://www.unsatisfactorysoftware.co.uk
 */

#include <stdio.h>
#include <stdint.h>

char hammed[16] = {	0x15, 0x02, 0x49, 0x5e,
					0x64, 0x73, 0x38, 0x2f,
					0xd0, 0xc7, 0x8c, 0x9b,
					0xa1, 0xb6, 0xfd, 0xea
};

uint8_t parity(uint8_t d)
{
/* nicked from raspi-teletext */

    d &= 0x7f;
    uint8_t p = 1;
    uint8_t t = d;
    int i;
    for (i=0; i<7; i++) {
        p += t&1;
        t = t>>1;
    }
    p &= 1;
    return d|(p<<7);

}

void pkt_header(char *buffer, int pkt, int mag, int tens, int page)
{
	int i = 2;

	if(pkt == 0) {
		buffer[0] = hammed[mag];
		buffer[1] = hammed[tens];
		buffer[2] = hammed[page];
		buffer[3] = hammed[0];
		buffer[4] = hammed[0];
		buffer[5] = hammed[0];
		buffer[6] = hammed[0];
		buffer[7] = hammed[0];
		i = 8;
	} else {
		if(pkt%2 == 0) { //even
			buffer[0] = hammed[mag];
		} else {
			buffer[0] = hammed[mag + 8];
		}
		buffer[1] = hammed[pkt/2];
	}

	while(i < 42) {
		buffer[i] = parity(buffer[i]);
		i++;
	}
}
