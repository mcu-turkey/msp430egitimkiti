#include <stdarg.h>
#include <stdint.h>
#include "print.h"
#define PAD_RIGHT    	0x01
#define PAD_ZERO     	0x02
#define PRINT_BUF_LEN 	12
#include "../drivers/spi.h"
#include "../softserial.h"


int prints(char *string, unsigned char width, unsigned char pad)       
{
       int pc = 0;
       unsigned char padchar = ' ';                           // The pading char is space normally

       if (width > 0)                                                                           // If output width is defined
       {                                   
              unsigned char len = 0;
              char *ptr;
              for (ptr = string; *ptr; ++ptr) ++len;      // Calculate string length and put it in len
              if (len >= width) width = 0;    // If string is longer than width, then width is not applicable define as zero
              else width -= len;              // Else redefine width as padding spaces
              if (pad & PAD_ZERO) padchar = '0';      // If padding char is zero, then get padchar as zero ready instead of original space
       }
       if (!(pad & PAD_RIGHT))                                // If not right padding - left justification
       {                                                  
              for (; width > 0; --width)               // If ther is padding width. Output padding char as '0' or ' '.
              {  
                     //putChar (padchar);
                      SoftSerial_xmit(padchar);
                     ++pc;
              }
       }
       for (; *string ; ++string)                      // Output the full string
       {          
              //putChar (*string);
               SoftSerial_xmit(*string);
              ++pc;
       }
       for (; width > 0; --width) {      // Write padding char to the right if normal left justification
              //putChar (padchar);
              SoftSerial_xmit(padchar);
              ++pc;
       }
       return pc;                                                                 // Return the output char number
}


#define PRINT_BUF_LEN 12

int printi(long int i, unsigned char b, unsigned char sg, unsigned char width, unsigned char pad, unsigned char letbase)
{
       char print_buf[PRINT_BUF_LEN];                         // Interger as string array
       char *s;
       char neg = 0;
       unsigned long int t;
       unsigned long int u = i;
       int pc = 0;

       if (i == 0)                                                                       // If output char is 0, then just output it with padding and width.
       {                      
              print_buf[0] = '0';
              print_buf[1] = '\0';                     // Always remenber to put string end
              return prints(print_buf, width, pad);           //Print out zero and done.
       }

       if (sg && (b == 10) && (i < 0))                 // If it is a negative int, then record the '-' and number as positive
       {             
              neg = 1;
              u = -i;
       }

       s = print_buf + PRINT_BUF_LEN-1;  // Point s to the end of the output buffer and put a null there.
       *s = '\0';

       while (u)                                // Convert the positive int to string with whatever counting base, dec, or hex.
       {                         
              t = u % b;
              if( t >= 10 )
                     t += letbase - '0' - 10;
              *--s = t + '0';
              u /= b;
       }

       if (neg)
       {                          // If it is a negative number
              if( width && (pad & PAD_ZERO) )
              {     // If there is width, right justified and pad with zero, output negative sign.
                     //putChar ('-');
                     SoftSerial_xmit('-');
                     ++pc;
                     --width;
              }
              else *--s = '-';                  // Otherwise put the '-' to string buffer.
                    
       }
       return pc + prints (s, width, pad);          // Output the string buffer and return the output counter.
}

int printf(char *format, ...)
{
	int width, pad;
	int pc = 0;
	char scr[2];
	 va_list args;
	 va_start(args, format);
	 for (; *format != 0; ++format) 
	 {
	      if (*format == '%') 
	      {
	             ++format;
	             width = pad = 0;
	             if (*format == '\0') break;
	             if (*format == '%') goto out;
	             /*if (*format == '-') 
	             {
	                   ++format;
	                   pad = PAD_RIGHT;
	             }
	             while (*format == '0') 
	             {
	                   ++format;
	                   pad |= PAD_ZERO;
	             }*/
	             for ( ; *format >= '0' && *format <= '9'; ++format) 
	             {
	                    width *= 10;
	                   width += *format - '0';
	             }
	             if( *format == 's' )
	             {
	                   char *s = (char *)va_arg( args, int );
	                   pc += prints (s?s:"(null)", width, pad);
	                   continue;
	             }
	             if( *format == 'd' ) 
	             {
	                   pc += printi (va_arg( args, long int ), 10, 1, width, pad, 'a');
	                   continue;
	             }
                 if( *format == 'x' ) 
                 {
                       pc += printi (va_arg( args, long int ), 16, 0, width, pad, 'a');
                       continue;
                 }
                 if( *format == 'X' ) 
                 {
                       pc += printi (va_arg( args, long int ), 16, 0, width, pad, 'A');
                       continue;
                 }
                 if( *format == 'u' ) 
                 {
                              pc += printi (va_arg( args, long int ), 10, 0, width, pad, 'a');
                       continue;
                 }
                 if( *format == 'c' )
                 {                                 // char are converted to int then pushed on the stack
                       scr[0] = (char)va_arg( args, int );
                       scr[1] = '\0';
                       pc += prints (scr, width, pad);
                       continue;
                 }
        }else
        {        out:
                //putChar(*format);
                SoftSerial_xmit(*format);
                ++pc;
           	             
	      }
	}
	return 0;
}
