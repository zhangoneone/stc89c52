/*------------------------------------------------------------------------/
/  Universal string handler for user console interface
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "xprintf.h"
#include "spin_common.h"
#if _USE_XFUNC_OUT
//#include <stdarg.h>
//volatile void (*xfunc_out)(unsigned char);	/* Pointer to the output stream */
DATA unsigned int xfunc_out;
static DATA char *outptr;

/*----------------------------------------------*/
/* Put a character                              */
/*----------------------------------------------*/

void xputc (char c)
{
	if (_CR_CRLF && c == '\n') xputc('\r');		/* CR -> CRLF */

	if (outptr) {
		*outptr++ = (unsigned char)c;
		return;
	}

	if (xfunc_out) ((xfunc_out_t)xfunc_out)((unsigned char)c);
}



/*----------------------------------------------*/
/* Put a null-terminated string                 */
/*----------------------------------------------*/

void xputs (					/* Put a string to the default device */
	const char* str				/* Pointer to the string */
)
{
	while (*str)
		xputc(*str++);
}


#endif /* _USE_XFUNC_OUT */



#if _USE_XFUNC_IN
//unsigned char (*xfunc_in)(void);	/* Pointer to the input stream */
DATA unsigned int xfunc_in;
/*----------------------------------------------*/
/* Get a character from the input,if empty then block */
/*----------------------------------------------*/

char xgetc(){
	if (!xfunc_in) return 0;		/* No input function specified */
	return ((xfunc_in_t)xfunc_in)();
}
/*----------------------------------------------*/
/* Get a line from the input                    */
/*----------------------------------------------*/

int xgets (		/* 0:End of stream, 1:A line arrived */
	char* buff,	/* Pointer to the buffer */
	int len		/* Buffer length */
)
{
	int c, i;


	if (!xfunc_in) return 0;		/* No input function specified */

	i = 0;
	for (;;) {
		c = ((xfunc_in_t)xfunc_in)();				/* Get a char from the incoming stream */
		if (!c) return 0;			/* End of stream? */
		if (c == '\r') break;		/* End of line? */
		if ((c == '\b'||c == 0x7f) && i) {		/* Back space? 电脑输入的backspace键值是0x7f*/
			i--;
#if _LINE_ECHO
			xputc(c);
#endif
			continue;
		}
		if (c >= ' ' && i < len - 1) {	/* Visible chars */
			buff[i++] = c;
#if _LINE_ECHO
			xputc(c);
#endif
		}
	}
	buff[i] = 0;	/* Terminate with a \0 */
#if _LINE_ECHO
#if !_LF_CRLF
		xputc('\r');
#endif
		xputc('\n');
#endif
	return 1;
}

#endif /* _USE_XFUNC_IN */
