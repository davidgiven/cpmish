#define _POSIX_C_SOURCE 199309
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <z80ex/z80ex_dasm.h>
#include "globals.h"

Z80EX_CONTEXT* z80;
uint8_t ram[0x10000];

struct watchpoint
{
	uint16_t address;
	uint8_t value;
	bool enabled;
};

static uint16_t breakpoints[16];
static struct watchpoint watchpoints[16];
static bool tracing = false;
static bool singlestepping = true;
static bool bdosbreak = false;


static uint8_t read_cb(Z80EX_CONTEXT* z80, uint16_t addr, int m1_state, void* data)
{
	return ram[addr];
}

static uint8_t dasm_read_cb(uint16_t addr, void* data)
{
	return ram[addr];
}

static void write_cb(Z80EX_CONTEXT* z80, uint16_t addr, uint8_t value, void* data)
{
	ram[addr] = value;
}

static uint8_t ioread_cb(Z80EX_CONTEXT* z80, uint16_t addr, void* data)
{
	return 0;
}

static void iowrite_cb(Z80EX_CONTEXT* z80, uint16_t addr, uint8_t value, void* data)
{
	biosbdos_entry(addr & 0xff);
	if (bdosbreak)
		singlestepping = true;
}

static uint8_t irqread_cb(Z80EX_CONTEXT* z80, void* user)
{
	return 0;
}

void showregs(void)
{
	uint16_t af = z80ex_get_reg(z80, regAF);
	printf("%c%c.%c.%c%c%c sp=%04x af=%04x bc=%04x de=%04x hl=%04x ix=%04x iy=%04x\n",
		(af & 0x80) ? 'S' : 's',
		(af & 0x40) ? 'Z' : 'z',
		(af & 0x10) ? 'H' : 'h',
		(af & 0x04) ? 'P' : 'p',
		(af & 0x02) ? 'N' : 'n',
		(af & 0x01) ? 'C' : 'c',
		z80ex_get_reg(z80, regSP),
		af,
		z80ex_get_reg(z80, regBC),
		z80ex_get_reg(z80, regDE),
		z80ex_get_reg(z80, regHL),
		z80ex_get_reg(z80, regIX),
		z80ex_get_reg(z80, regIY));

	char buffer[80];
	int tstates;
	uint16_t pc = z80ex_get_reg(z80, regPC);
	z80ex_dasm(buffer, sizeof(buffer), 0, &tstates, &tstates, dasm_read_cb, pc, NULL);
	printf("%04x : %s\n", pc, buffer);
}

static void cmd_register(void)
{
	char* w1 = strtok(NULL, " ");
	char* w2 = strtok(NULL, " ");

	if (w1 && w2)
	{
		Z80_REG_T reg = -1;
		if (strcmp(w1, "sp") == 0)
			reg = regSP;
		else if (strcmp(w1, "pc") == 0)
			reg = regPC;
		else if (strcmp(w1, "af") == 0)
			reg = regAF;
		else if (strcmp(w1, "bc") == 0)
			reg = regBC;
		else if (strcmp(w1, "de") == 0)
			reg = regDE;
		else if (strcmp(w1, "hl") == 0)
			reg = regHL;
		else if (strcmp(w1, "ix") == 0)
			reg = regIX;
		else if (strcmp(w1, "iy") == 0)
			reg = regIY;
		else
		{
			printf("Bad register\n");
			return;
		}

		z80ex_set_reg(z80, reg, strtoul(w2, NULL, 16));
	}

	showregs();
}

static void cmd_break(void)
{
	char* w1 = strtok(NULL, " ");
	if (w1)
	{
		uint16_t breakpc = strtoul(w1, NULL, 16);
		for (int i=0; i<sizeof(breakpoints)/sizeof(*breakpoints); i++)
		{
			if (breakpoints[i] == 0xffff)
			{
				breakpoints[i] = breakpc;
				return;
			}
		}
		printf("Too many breakpoints\n");
	}
	else
	{
		for (int i=0; i<sizeof(breakpoints)/sizeof(*breakpoints); i++)
		{
			if (breakpoints[i] != 0xffff)
				printf("%04x\n", breakpoints[i]);
		}
	}
}

static void cmd_watch(void)
{
	char* w1 = strtok(NULL, " ");
	if (w1)
	{
		uint16_t watchaddr = strtoul(w1, NULL, 16);
		for (int i=0; i<sizeof(watchpoints)/sizeof(*watchpoints); i++)
		{
			struct watchpoint* w = &watchpoints[i];
			if (!w->enabled)
			{
				w->address = watchaddr;
				w->enabled = true;
				w->value = ram[watchaddr];
				return;
			}
		}
		printf("Too many breakpoints\n");
	}
	else
	{
		for (int i=0; i<sizeof(watchpoints)/sizeof(*watchpoints); i++)
		{
			struct watchpoint* w = &watchpoints[i];
			if (w->enabled)
				printf("%04x (current value: %02x)\n", w->address, w->value);
		}
	}
}

static void cmd_delete_breakpoint(void)
{
	char* w1 = strtok(NULL, " ");
	if (w1)
	{
		uint16_t breakpc = strtoul(w1, NULL, 16);
		for (int i=0; i<sizeof(breakpoints)/sizeof(*breakpoints); i++)
		{
			if (breakpoints[i] == breakpc)
			{
				breakpoints[i] = 0xffff;
				return;
			}
		}
		printf("No such breakpoint\n");
	}
}

static void cmd_delete_watchpoint(void)
{
	char* w1 = strtok(NULL, " ");
	if (w1)
	{
		uint16_t address = strtoul(w1, NULL, 16);
		for (int i=0; i<sizeof(breakpoints)/sizeof(*breakpoints); i++)
		{
			struct watchpoint* w = &watchpoints[i];
			if (w->enabled && (w->address == address))
			{
				w->enabled = false;
				return;
			}
		}
		printf("No such watchpoint\n");
	}
}

static void cmd_memory(void)
{
	char* w1 = strtok(NULL, " ");
	char* w2 = strtok(NULL, " ");

	if (!w2)
		w2 = "100";

	if (w1 && w2)
	{
		uint16_t startaddr = strtoul(w1, NULL, 16);
		uint16_t endaddr = startaddr + strtoul(w2, NULL, 16);
		uint16_t startrounded = startaddr & ~0xf;
		uint16_t endrounded = (endaddr + 0xf) & ~0xf;

		uint16_t p = startrounded;

		while (p < endrounded)
		{
			printf("%04x : ", p);
			for (int i = 0; i < 16; i++)
			{
				uint16_t pp = p + i;
				if ((pp >= startaddr) && (pp < endaddr))
					printf("%02x ", ram[pp]);
				else
					printf("   ");
			}
			printf(": ");
			for (int i = 0; i < 16; i++)
			{
				uint16_t pp = p + i;
				if ((pp >= startaddr) && (pp < endaddr))
				{
					uint8_t c = ram[pp];
					if ((c < 32) || (c > 127))
						c = '.';
					putchar(c);
				}
				else
					putchar(' ');
			}
			p += 16;
			putchar('\n');
		}
	}
}

static void cmd_bdos(void)
{
	char* w1 = strtok(NULL, " ");
	if (w1)
		bdosbreak = !!strtoul(w1, NULL, 16);
	else
		printf("break on bdos entry: %s\n", bdosbreak ? "on" : "off");
}

static void cmd_tracing(void)
{
	char* w1 = strtok(NULL, " ");
	if (w1)
		tracing = !!strtoul(w1, NULL, 16);
	else
		printf("tracing: %s\n", tracing ? "on" : "off");
}

static void cmd_help(void)
{
	printf("Sleazy debugger\n"
	       "  r               show registers\n"
		   "  r <reg> <value> set register\n"
		   "  b               show breakpoints\n"
		   "  b <addr>        set breakpoint\n"
		   "  db <addr>       delete breakpoint\n"
		   "  w <addr>        set watchpoint\n"
		   "  dw <addr>       delete watchpoint\n"
		   "  m <addr> <len>  show memory\n"
		   "  s               single step\n"
		   "  g               continue\n"
		   "  bdos 0|1        enable break on bdos entry\n"
		   "  trace 0|1       enable tracing\n"
	);
}

static void debug(void)
{
	bool go = false;
	showregs();
	while (!go)
	{
		char* cmdline = readline("debug>");
		if (!cmdline)
			exit(0);

		char* token = strtok(cmdline, " ");
		if (token != NULL)
		{
			if (strcmp(token, "?") == 0)
				cmd_help();
			else if (strcmp(token, "r") == 0)
				cmd_register();
			else if (strcmp(token, "b") == 0)
				cmd_break();
			else if (strcmp(token, "w") == 0)
				cmd_watch();
			else if (strcmp(token, "db") == 0)
				cmd_delete_breakpoint();
			else if (strcmp(token, "dw") == 0)
				cmd_delete_watchpoint();
			else if (strcmp(token, "m") == 0)
				cmd_memory();
			else if (strcmp(token, "s") == 0)
			{
				singlestepping = true;
				go = true;
			}
			else if (strcmp(token, "g") == 0)
			{
				singlestepping = false;
				go = true;
			}
			else if (strcmp(token, "bdos") == 0)
				cmd_bdos();
			else if (strcmp(token, "tracing") == 0)
				cmd_tracing();
			else
				printf("Bad command\n");
		}

		free(cmdline);
	}
}

static void sigusr1_cb(int number)
{
	singlestepping = true;
}

void emulator_init(void)
{
	for (int i=0; i<sizeof(breakpoints)/sizeof(*breakpoints); i++)
		breakpoints[i] = 0xffff;

	z80 = z80ex_create(
		read_cb, NULL,
		write_cb, NULL,
		ioread_cb, NULL,
		iowrite_cb, NULL,
		irqread_cb, NULL);

	singlestepping = flag_enter_debugger;

	struct sigaction action = {
		.sa_handler = sigusr1_cb
	};
	sigaction(SIGUSR1, &action, NULL);
}

void emulator_run(void)
{
	for (;;)
	{
		uint16_t pc = z80ex_get_reg(z80, regPC);
		if (!singlestepping)
		{
			for (int i=0; i<sizeof(breakpoints)/sizeof(*breakpoints); i++)
				if (pc == breakpoints[i])
					singlestepping = true;
		}
		for (int i=0; i<sizeof(watchpoints)/sizeof(*watchpoints); i++)
		{
			struct watchpoint* w = &watchpoints[i];
			if (w->enabled && (ram[w->address] != w->value))
			{
				printf("\nWatchpoint hit: %04x has changed from %02x to %02x\n",
					w->address, w->value, ram[w->address]);
				w->value = ram[w->address];
				singlestepping = true;
			}
		}

		if (singlestepping && !z80ex_last_op_type(z80))
			debug();
		else if (tracing)
			showregs();

		z80ex_step(z80);
	}
}

