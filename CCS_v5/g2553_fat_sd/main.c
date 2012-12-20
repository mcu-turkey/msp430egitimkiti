/**
 * main.c - test driver program for Petit FatFS for msp430g2553
 *
 * Version: 1.00 Initial version 04-20-2011
 * Version: 1.01 cleanup 04-21-2011
 * Version: 1.02 added write support 04-05-2012
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "softserial.h"
#include "pff2a/src/diskio.h"
#include "pff2a/src/pff.h"
#include "print/print.h"
#include "drivers/spi.h"

char Line[64]; // serial and general buffer

static
void put_rc (FRESULT rc)
{
	const char *p;
	static const char str[] =
		"OK\0" "DISK_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"NOT_OPENED\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0";
	FRESULT i;

	for (p = str, i = 0; i != rc && *p; i++) {
		while(*p++) ;
	}
	printf("rc=%u FR_%s\n", (long int)rc, p);
}

#if 1
static
void put_dump (
	const void* buff,		/* Pointer to the byte array to be dumped */
	unsigned long addr,		/* Heading address value */
	int len					/* Number of bytes to be dumped */
)
{
	int i;
	const unsigned char *p = buff;

#if 0
	printf("%8lX ", addr);		/* address */


	for (i = 0; i < len; i++)		/* data (hexdecimal) */
		printf(" %2X", p[i]);
#endif
	SoftSerial_xmit(' ');
	for (i = 0; i < len; i++)		/* data (ascii) */
		SoftSerial_xmit((p[i] >= ' ' && p[i] <= '~') ? p[i] : '.');

	SoftSerial_xmit('\n');
}
#endif

/**
 * setup() - initialize timers and clocks
 */

void setup() {
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

    DCOCTL = 0x00;                  // Set DCOCLK to 16MHz
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
    //DCOCTL += 6; // the default calibrated clock on my mcu is slow

    SoftSerial_init();              // Configure TIMERA serial TX P1.1 RX P1.2

    P1DIR |= BIT4; P1SEL |= BIT4;   // output SMCLK for measurement on P1.4
}

/**
 * loop() - this routine runs over and over
 *
 * Wait for data to arrive. When something is available,
 * read it from the ring_buffer and echo it back to the
 * sender.
 */

void loop() {
    int c;

    if ( !SoftSerial_empty() ) {
        while((c=SoftSerial_read()) != -1) {
            SoftSerial_xmit((uint8_t)c);
        }
    }
}

static void put_drc(long int res)
{
	printf("rc=%d\n", res);
}

static void get_line(char *buff, BYTE len) {
	BYTE c, i;

	i = 0;
	for (;;) {
		if (!SoftSerial_empty()) {
			c = SoftSerial_read();
			if (c == '\r')
				break;
			if ((c == '\b') && i)
				i--;
			if ((c >= ' ') && (i < len - 1))
				buff[i++] = c;
		}
	}
	buff[i] = 0;
	//xmit('\n'); /* for echo */
}


/**
 * main - sample program main loop
 *
 * 'di' - disk initialize
 * 'fi' - FATFS initialize
 * 'fl' - list directory
 * 'fo filename.txt' - open file
 * 'ft' - type file to serial port
 * 'fd' - dump file to serial port
 * 'fw' - write to file (write.txt) must exist before writing
 *
 */
void main (void)
{
	char *ptr;
	FRESULT res;
	WORD s1, s2, ofs;
	FATFS fs;			/* File system object */
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */
	
	setup();
	spi_initialize();

        _enable_interrupt(); // enable timers for serial isr handlers

	printf("\nPFF test monitor\ncommands are: di,fi,fo,fd,fl,ft,fw\n");
		
	for (;;) {
		printf("> ");
		get_line(Line, sizeof(Line));
		ptr = Line;
		switch (*ptr++) {
		case 'd':
			switch (*ptr++) {
			case 'i': /* di - Initialize physical drive */
				printf("Disk Initialize...\n");
				res = disk_initialize();
				put_drc(res);
				break;
			}
			break;
		case 'f':
			switch (*ptr++) {
			case 'i': /* fi - Mount the volume */
				put_rc(pf_mount(&fs));
				break;

			case 'o': /* fo <file> - Open a file */
				while (*ptr == ' ')
					ptr++;
				put_rc(pf_open(ptr));
				break;

#if _USE_READ
			case 'd': /* fd - Read the file 128 bytes and dump it */
				ofs = fs.fptr;
				res = pf_read(Line, sizeof(Line), &s1);
				if (res != FR_OK) {
					put_rc(res);
					break;
				}
				ptr = Line;
				while (s1) {
					s2 = (s1 >= 16) ? 16 : s1;
					s1 -= s2;
					put_dump((BYTE*) ptr, ofs, s2);
					ptr += 16;
					ofs += 16;
				}
				break;

			case 't': /* ft - Type the file data via dreadp function */
				do {
					res = pf_read(0, 32768, &s1);
					if (res != FR_OK) {
						put_rc(res);
						break;
					}
				} while (s1 == 32768);
				break;
#endif

#if _USE_DIR
			case 'l': /* fl [<path>] - Directory listing */
				while (*ptr == ' ')
					ptr++;
				res = pf_opendir(&dir, ptr);
				if (res) {
					put_rc(res);
					break;
				}
				s1 = 0;
				for (;;) {
					res = pf_readdir(&dir, &fno);
					if (res != FR_OK) {
						put_rc(res);
						break;
					}
					if (!fno.fname[0])
						break;
					if (fno.fattrib & AM_DIR )
						printf("   <DIR>   %s\n", fno.fname);
					else
						printf("%9u  %s\n", fno.fsize, fno.fname);
					s1++;
				}
				printf("%u item(s)\n", s1);
				break;
#endif

#if _USE_WRITE
			case 'w': /* fw - write into write.txt, see write limitation in Petit FatFS documentation */
				printf("\nTrying to open an existing file for writing (write.txt)...\n");
				res = pf_open("WRITE.TXT");
				if (res != FR_OK) {
					put_rc(res);
				}
				else {
					WORD bw;
					WORD n = 0;

					printf("Writing 100 lines of text data\n");

					do {
						ltoa(++n,Line);
						strcat(Line," The quick brown fox jumps over the lazy dog. 1234567890\r\n");
						res = pf_write(Line, strlen(Line), &bw);
						if (res != FR_OK) {
							put_rc(res);
							break;
						}
						if ( n & 0x10 ) {
							printf(".");
						}
					} while(n < 100);

					if ( res == FR_OK ) {
						printf("\nFinalizing the file write process.\n");
						res = pf_write(0, 0, &bw);
						if (res != FR_OK)
							put_rc(res);
						}
					}
				break;
#endif

			}
			break;
		} /* end switch */
	} /* end for */
}
