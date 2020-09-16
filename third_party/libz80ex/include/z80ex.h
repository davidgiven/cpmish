/*
 * Z80Ex, ZILoG Z80 CPU emulator.
 *
 * by Pigmaker57 aka boo_boo [pigmaker57@kahoh57.info]
 *
 * contains some code from the FUSE project (http://fuse-emulator.sourceforge.net)
 * Released under GNU GPL v2
 *
 */

#ifndef _Z80EX_H_INCLUDED
#define _Z80EX_H_INCLUDED

#include "z80ex_common.h"

typedef
enum {regAF,regBC,regDE,regHL,regAF_,regBC_,regDE_,regHL_,regIX,regIY,regPC,regSP,regI,regR,regR7,regIM/*0,1 or 2*/,regIFF1,regIFF2}
Z80_REG_T;

typedef struct {
	int API_revision;
	int major;
	int minor;
	char *release_type; /*e.g., "beta", "RC"*/
	char *as_string; /*full version string, e.g., "0.16.7beta"*/
} Z80EX_VERSION;

#ifndef __Z80EX_SELF_INCLUDE

struct _z80_cpu_context;
typedef struct _z80_cpu_context Z80EX_CONTEXT;

#endif

/*callback prototypes:*/

/*to be called on each T-State [optional]*/
typedef void (*z80ex_tstate_cb)(Z80EX_CONTEXT *cpu, void *user_data);

/*read byte from memory <addr> -- called when RD & MREQ goes active.
m1_state will be 1 if M1 signal is active*/
typedef Z80EX_BYTE (*z80ex_mread_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data);

/*write <value> to memory <addr> -- called when WR & MREQ goes active*/
typedef void (*z80ex_mwrite_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);

/*read byte from <port> -- called when RD & IORQ goes active*/
typedef Z80EX_BYTE (*z80ex_pread_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data);

/*write <value> to <port> -- called when WR & IORQ goes active*/
typedef void (*z80ex_pwrite_cb)(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data);

/*read byte of interrupt vector -- called when M1 and IORQ goes active*/
typedef Z80EX_BYTE (*z80ex_intread_cb)(Z80EX_CONTEXT *cpu, void *user_data);

/*called when the RETI instruction is executed (useful for emulating Z80 PIO/CTC and such)*/
typedef void (*z80ex_reti_cb)(Z80EX_CONTEXT *cpu, void *user_data);

#ifndef __Z80EX_SELF_INCLUDE
	
#ifdef __cplusplus
extern "C" {
#endif

/*get version info*/
extern Z80EX_VERSION *z80ex_get_version();

/*create and initialize CPU*/
extern Z80EX_CONTEXT *z80ex_create(z80ex_mread_cb mrcb_fn, void *mrcb_data,
	z80ex_mwrite_cb mwcb_fn, void *mwcb_data,
	z80ex_pread_cb prcb_fn, void *prcb_data,
	z80ex_pwrite_cb pwcb_fn, void *pwcb_data,
	z80ex_intread_cb ircb_fn, void *ircb_data);

/*destroy CPU*/
extern void z80ex_destroy(Z80EX_CONTEXT *cpu);

/*do next opcode (instruction or prefix), return number of T-states*/
extern int z80ex_step(Z80EX_CONTEXT *cpu);

/*returns type of the last opcode, processed with z80ex_step.
type will be 0 for complete instruction, or dd/fd/cb/ed for opcode prefix.*/
extern Z80EX_BYTE z80ex_last_op_type(Z80EX_CONTEXT *cpu);
	
/*set T-state callback*/
extern void z80ex_set_tstate_callback(Z80EX_CONTEXT *cpu, z80ex_tstate_cb cb_fn, void *user_data);

/*set RETI callback*/
extern void z80ex_set_reti_callback(Z80EX_CONTEXT *cpu, z80ex_reti_cb cb_fn, void *user_data);

/*set memory read callback*/
extern void z80ex_set_memread_callback(Z80EX_CONTEXT *cpu, z80ex_mread_cb mrcb_fn, void *mrcb_data);

/*set memory write callback*/
extern void z80ex_set_memwrite_callback(Z80EX_CONTEXT *cpu, z80ex_mwrite_cb mwcb_fn, void *mwcb_data);

/*set port read callback*/
extern void z80ex_set_portread_callback(Z80EX_CONTEXT *cpu, z80ex_pread_cb prcb_fn, void *prcb_data);

/*set port write callback*/
extern void z80ex_set_portwrite_callback(Z80EX_CONTEXT *cpu, z80ex_pwrite_cb pwcb_fn, void *pwcb_data);

/*set INT read callback*/
extern void z80ex_set_intread_callback(Z80EX_CONTEXT *cpu, z80ex_intread_cb ircb_fn, void *ircb_data);

/*maskable interrupt*/
/*returns number of T-states if interrupt was accepted, otherwise 0*/
extern int z80ex_int(Z80EX_CONTEXT *cpu);

/*non-maskable interrupt*/
/*returns number of T-states (11 if interrupt was accepted, or 0 if processor
is doing an instruction right now)*/
extern int z80ex_nmi(Z80EX_CONTEXT *cpu);

/*reset CPU*/
extern void z80ex_reset(Z80EX_CONTEXT *cpu);

/*get register value*/
extern Z80EX_WORD z80ex_get_reg(Z80EX_CONTEXT *cpu, Z80_REG_T reg);

/*set register value (for 1-byte registers lower byte of <value> will be used)*/
extern void z80ex_set_reg(Z80EX_CONTEXT *cpu, Z80_REG_T reg, Z80EX_WORD value);

/*returns 1 if CPU doing HALT instruction now*/
extern int z80ex_doing_halt(Z80EX_CONTEXT *cpu);

/*when called from callbacks, returns current T-state of the executing opcode (instruction or prefix),
else returns T-states taken by last opcode executed*/
extern int z80ex_op_tstate(Z80EX_CONTEXT *cpu);

/*generate <w_states> Wait-states. (T-state callback will be called <w_states> times, when defined).
should be used to simulate WAIT signal or disabled CLK*/ 
extern void z80ex_w_states(Z80EX_CONTEXT *cpu, unsigned w_states);

/*spend one T-state doing nothing (often IO devices cannot handle data request on
the first T-state at which RD/WR goes active).
for I/O callbacks*/
extern void z80ex_next_t_state(Z80EX_CONTEXT *cpu);

/*returns 1 if maskable interrupts are possible in current z80 state*/
extern int z80ex_int_possible(Z80EX_CONTEXT *cpu);

/*returns 1 if non-maskable interrupts are possible in current z80 state*/
extern int z80ex_nmi_possible(Z80EX_CONTEXT *cpu);

#ifdef __cplusplus
}
#endif

#endif

#endif
