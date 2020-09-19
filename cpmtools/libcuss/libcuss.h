/* libcuss © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#ifndef LIBCUSS_H
#define LIBCUSS_H

#include <stdint.h>

extern uint16_t screenx;
extern uint16_t screeny;

extern void con_goto(uint16_t x, uint16_t y);
extern void con_clear(void);
extern void con_putc(uint16_t c);
extern void con_puts(const char* s);
extern uint8_t con_getc(void);
extern void con_newline(void);
extern void con_clear_to_eol(void);
extern void con_revon(void);
extern void con_revoff(void);

#if defined LIBCUSS_KAYPROII
    #define SCREENWIDTH 80
    #define SCREENHEIGHT 24
    #define LIBCUSS_ADM3
#elif defined LIBCUSS_NC200
    #define SCREENWIDTH 80
    #define SCREENHEIGHT 18
    #define LIBCUSS_VT52
#elif defined LIBCUSS_BROTHEROP2
    #define SCREENWIDTH 80
    #define SCREENHEIGHT 14
    #define LIBCUSS_ADM3
#elif defined LIBCUSS_BROTHER_WP2450DS
	#define SCREENWIDTH 91
	#define SCREENHEIGHT 26
    #define LIBCUSS_ADM3
#elif defined LIBCUSS_SPECTRUM_PLUS_THREE
	#define SCREENWIDTH 51
	#define SCREENHEIGHT 24
	#define LIBCUSS_VT52
#elif defined LIBCUSS_SPECTRUM_NEXT
	#define SCREENWIDTH 80
	#define SCREENHEIGHT 24
	#define LIBCUSS_ANSI
#else
    #error "No libcuss configuration specified."
#endif

/* Generic terminal information */

#if defined LIBCUSS_ADM3
    #define LIBCUSS_CLEAR "\032"
    #define LIBCUSS_CLEAREOL "\030"
	#define LIBCUSS_REVON "\033B0"
	#define LIBCUSS_REVOFF "\033C0"
    #define LIBCUSS_NEWLINE "\r\n"

    #define LIBCUSS_KEY_LEFT "\010"
    #define LIBCUSS_KEY_DOWN "\012"
    #define LIBCUSS_KEY_UP "\013"
    #define LIBCUSS_KEY_RIGHT "\014"
#endif

#if defined LIBCUSS_VT52
	#define LIBCUSS_CLEAR "\033H\033J"
	#define LIBCUSS_CLEAREOL "\033K"
	#define LIBCUSS_NEWLINE "\r\n"
	#define LIBCUSS_REVON "\033p"
	#define LIBCUSS_REVOFF "\033q"

    #define LIBCUSS_KEY_LEFT "\010"
    #define LIBCUSS_KEY_DOWN "\012"
    #define LIBCUSS_KEY_UP "\013"
    #define LIBCUSS_KEY_RIGHT "\014"
	//#define LIBCUSS_KEY_LEFT "\033[D"
	//#define LIBCUSS_KEY_DOWN "\033[B"
	//#define LIBCUSS_KEY_UP "\033[A"
	//#define LIBCUSS_KEY_RIGHT "\033[C"
#endif

#if defined LIBCUSS_ANSI
	#define LIBCUSS_CLEAR "\033[2J"
	#define LIBCUSS_CLEAREOL "\033[K"
	#define LIBCUSS_NEWLINE "\r\n"
	#define LIBCUSS_REVON "\033[7m"
	#define LIBCUSS_REVOFF "\033[0m"

    #define LIBCUSS_KEY_LEFT "\010"
    #define LIBCUSS_KEY_DOWN "\012"
    #define LIBCUSS_KEY_UP "\013"
    #define LIBCUSS_KEY_RIGHT "\014"
#endif

#endif

