#ifndef PRINT_H_
#define PRINT_H_

int prints(char *string, unsigned char width, unsigned char pad);
int printi(long int i, unsigned char b, unsigned char sg, unsigned char width, 
			unsigned char pad, unsigned char letbase);
int printf(char *format, ...);			
#endif /*PRINT_H_*/
