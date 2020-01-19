/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */

#define _USE_XFUNC_IN	1	/* 1: Use input function */
#define	_LINE_ECHO		1	/* 1: Echo back input chars in xgets function */


#if _USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (func)
typedef void (*xfunc_out_t)(unsigned char);
extern unsigned int xfunc_out;
void xputc(char c);
void xputs(const char* str);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if _USE_XFUNC_IN
#define xdev_in(func) xfunc_in = (func)
typedef unsigned char (*xfunc_in_t)(void);
extern unsigned int xfunc_in;
char xgetc();
int xgets (char* buff, int len);
int xatoi (char** str, long* res);
#endif

#endif
