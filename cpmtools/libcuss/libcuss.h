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
extern void con_newline(void);
extern void con_clear_to_eol(void);

#if defined LIBCUSS_KAYPROII
    #define SCREENWIDTH 80
    #define SCREENHEIGHT 24
    #define LIBCUSS_ADM3
#elif defined LIBCUSS_NC200
    #define SCREENWIDTH 80
    #define SCREENHEIGHT 18
    #define LIBCUSS_ADM3
#elif defined LIBCUSS_BROTHEROP2
    #define SCREENWIDTH 80
    #define SCREENHEIGHT 14
    #define LIBCUSS_ADM3
#elif defined LIBCUSS_BROTHER_WP2450DS
	#define SCREENWIDTH 91
	#define SCREENHEIGHT 30
    #define LIBCUSS_ADM3
#else
    #error "No libcuss configuration specified."
#endif

/* Generic terminal information */

#if defined LIBCUSS_ADM3
    #define LIBCUSS_CLEAR "\032"
    #define LIBCUSS_CLEAREOL "\030"
    #define LIBCUSS_NEWLINE "\r\n"
    #define LIBCUSS_ADM3

    #define LIBCUSS_KEY_LEFT "\010"
    #define LIBCUSS_KEY_DOWN "\012"
    #define LIBCUSS_KEY_UP "\013"
    #define LIBCUSS_KEY_RIGHT "\014"
#endif

#endif
