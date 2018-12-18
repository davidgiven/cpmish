#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "globals.h"

#define FBASE 0xff00
#define COLDSTART (FBASE + 4) /* see bdos.asm */
#define CBASE 0xf700

static const uint8_t cpm_data[] =
#include ".obj/emucpm.cim.h"

static uint16_t dma;
static int exitcode = 0;

struct fcb
{
	cpm_filename_t filename; /* includes drive */
	uint8_t extent;
	uint8_t s1;
	uint8_t s2;
	uint8_t recordcount;
	uint8_t d[16];
	uint8_t currentrecord;
	uint8_t r[3];
};

static void bios_getchar(void);

static uint16_t get_de(void)
{
	return z80ex_get_reg(z80, regDE);
}

static uint8_t get_c(void)
{
	return z80ex_get_reg(z80, regBC);
}

static uint8_t get_d(void)
{
	return z80ex_get_reg(z80, regDE) >> 8;
}

static uint8_t get_e(void)
{
	return z80ex_get_reg(z80, regDE);
}

static uint8_t get_a(void)
{
	return z80ex_get_reg(z80, regAF) >> 8;
}

static void set_a(uint8_t a)
{
	uint16_t af = z80ex_get_reg(z80, regAF);
	af &= 0x00ff;
	af |= a << 8;
	z80ex_set_reg(z80, regAF, af);
}
	
static void set_result(uint16_t result)
{
	z80ex_set_reg(z80, regHL, result);

	uint16_t af = z80ex_get_reg(z80, regAF);
	af &= 0x00ff & ~(1<<6) & ~(1<<7);
	af |= result << 8;
	if (!result)
		af |= 1<<6;
	if (result & 0x80)
		af |= 1<<7;
	z80ex_set_reg(z80, regAF, af);

	uint16_t bc = z80ex_get_reg(z80, regBC);
	bc &= 0x00ff;
	bc |= result & 0xff00;
	z80ex_set_reg(z80, regBC, bc);
}

void bios_coldboot(void)
{
	memcpy(&ram[CBASE], cpm_data, sizeof(cpm_data));
	z80ex_set_reg(z80, regPC, COLDSTART);
}

static void bios_warmboot(void)
{
	dma = 0x0080;

	memcpy(&ram[CBASE], cpm_data, sizeof(cpm_data));
	z80ex_set_reg(z80, regPC, CBASE);

	if (user_command_line[0])
	{
		static bool terminate_next_time = false;
		if (terminate_next_time)
			exit(exitcode);
		terminate_next_time = true;

        z80ex_set_reg(z80, regPC, 0x0100);

        /* Push the magic exit code onto the stack. */
        z80ex_set_reg(z80, regSP, FBASE-4);
        ram[FBASE-4] = (FBASE-2) & 0xFF;
        ram[FBASE-3] = (FBASE-2) >> 8;
        ram[FBASE-2] = 0xD3; // out (??), a
        ram[FBASE-1] = 0xFE; // exit emulator

        int fd = open(user_command_line[0], O_RDONLY);
        if (fd == -1)
                fatal("couldn't open program: %s", strerror(errno));
        read(fd, &ram[0x0100], 0xFE00);
        close(fd);

		int offset = 1;
		for (int word = 1; user_command_line[word]; word++)
		{
			if (word > 1)
			{
				ram[0x0080 + offset] = ' ';
				offset++;
			}

			const char* pin = user_command_line[word];
			while (*pin)
			{
				if (offset > 125)
					fatal("user command line too long");
				ram[0x0080 + offset] = toupper(*pin++);
				offset++;
			}
		}
		ram[0x0080] = offset;
		ram[0x0080+offset] = 0;
	}
}

static void bios_const(void)
{
	struct pollfd pollfd = { 0, POLLIN, 0 };
	poll(&pollfd, 1, 0);
	set_a((pollfd.revents & POLLIN) ? 0xff : 0);
}

static void bios_getchar(void)
{
	char c = 0;
	(void) read(0, &c, 1);
	if (c == '\n')
		c = '\r';
	set_a(c);
}

static void bios_putchar(void)
{
	char c = get_c();
	(void) write(1, &c, 1);
}

static void bios_entry(uint8_t bios_call)
{
	switch (bios_call)
	{
		case 0: bios_coldboot();  return;
		case 1: bios_warmboot();  return;
		case 2: bios_const();     return; // const
		case 3: bios_getchar();   return; // conin
		case 4: bios_putchar();   return; // conout

		case 0xFE: exit(0); // magic emulator exit
	}

	showregs();
	fatal("unimplemented bios entry %d", bios_call);
}

static void bdos_getchar(void)
{
	bios_getchar();
	set_result(get_a());
}

static void bdos_putchar(void)
{
	uint8_t c = get_e();
	(void) write(1, &c, 1);
}

static void bdos_consoleio(void)
{
	uint8_t c = get_e();
	if (c == 0xff)
	{
		bios_const();
		if (get_a() == 0xff)
			bios_getchar();
	}
	else
		bdos_putchar();
}

static void bdos_printstring(void)
{
	uint16_t de = get_de();
	for (;;)
	{
		uint8_t c = ram[de++];
		if (c == '$')
			break;
		(void) write(1, &c, 1);
	}
}

static void bdos_consolestatus(void)
{
	bios_const();
	set_result(get_a());
}

void bdos_readline(void)
{
	fflush(stdout);

	uint16_t de = z80ex_get_reg(z80, regDE);
	uint8_t maxcount = ram[de+0];
	int count = read(0, &ram[de+2], maxcount);
	if ((count > 0) && (ram[de+2+count-1] == '\n'))
		count--;
	ram[de+1] = count;
	set_result(count);
}

static struct fcb* fcb_at(uint16_t address)
{
	struct fcb* fcb = (struct fcb*) &ram[address];

	/* Autoselect the current drive. */
	if (fcb->filename.drive == 0)
		fcb->filename.drive = ram[4] + 1;

	return fcb;
}

static struct fcb* find_fcb(void)
{
	return fcb_at(z80ex_get_reg(z80, regDE));
}

static int get_current_record(struct fcb* fcb)
{
	return (fcb->extent * 128) + fcb->currentrecord;
}

static void set_current_record(struct fcb* fcb, int record, int total)
{
	int extents = total / 128;
	fcb->extent = record / 128;
	if (fcb->extent < extents)
		fcb->recordcount = 128;
	else
		fcb->recordcount = total % 128;
	fcb->currentrecord = record % 128;
}

static void bdos_resetdisk(void)
{
	dma = 0x0080;
	ram[4] = 0; /* select drive A */
	set_result(0xff);
}

static void bdos_selectdisk(void)
{
	uint8_t e = get_e();
	ram[4] = e;
}

static void bdos_getdisk(void)
{
	set_result(ram[4]);
}

static void bdos_openfile(void)
{
	struct fcb* fcb = find_fcb();
	struct file* f = file_open(&fcb->filename);
	if (f)
	{
		set_current_record(fcb, 0, file_getrecordcount(f));
		set_result(0);
	}
	else
		set_result(0xff);
}

static void bdos_makefile(void)
{
	struct fcb* fcb = find_fcb();
	struct file* f = file_create(&fcb->filename);
	if (f)
	{
		set_current_record(fcb, 0, 0);
		set_result(0);
	}
	else
		set_result(0xff);
}

static void bdos_closefile(void)
{
	struct fcb* fcb = find_fcb();
	struct file* f = file_open(&fcb->filename);
	if (file_getrecordcount(f) < 128)
		file_setrecordcount(f, fcb->recordcount);
	int result = file_close(&fcb->filename);
	set_result(result ? 0xff : 0);
}

static void bdos_renamefile(void)
{
	struct fcb* srcfcb = fcb_at(z80ex_get_reg(z80, regDE));
	struct fcb* destfcb = fcb_at(z80ex_get_reg(z80, regDE)+16);
	int result = file_rename(&srcfcb->filename, &destfcb->filename);
	set_result(result ? 0xff : 0);
}

static void bdos_findnext(void)
{
	struct fcb* fcb = (struct fcb*) &ram[dma];
	memset(fcb, 0, sizeof(struct fcb));
	int i = file_findnext(&fcb->filename);
	set_result(i ? 0xff : 0);
}

static void bdos_findfirst(void)
{
	struct fcb* fcb = find_fcb();
	int i = file_findfirst(&fcb->filename);
	if (i == 0)
		bdos_findnext();
	else
		set_result(i ? 0xff : 0);
}

static void bdos_deletefile(void)
{
	struct fcb* fcb = find_fcb();
	int i = file_delete(&fcb->filename);
	set_result(i ? 0xff : 0);
}

typedef int readwrite_cb(struct file* f, uint8_t* ptr, uint16_t record);

static void bdos_readwritesequential(readwrite_cb* readwrite)
{
	struct fcb* fcb = find_fcb();

	struct file* f = file_open(&fcb->filename);
	int here = get_current_record(fcb);
	int i = readwrite(f, &ram[dma], here);
	set_current_record(fcb, here+1, file_getrecordcount(f));
	if (i == -1)
		set_result(0xff);
	else if (i == 0)
		set_result(1);
	else
		set_result(0);
}

static void bdos_readwriterandom(readwrite_cb* readwrite)
{
	struct fcb* fcb = find_fcb();

	uint16_t record = fcb->r[0] + (fcb->r[1]<<8);
	struct file* f = file_open(&fcb->filename);
	int i = readwrite(f, &ram[dma], record);
	set_current_record(fcb, record, file_getrecordcount(f));
	if (i == -1)
		set_result(0xff);
	else if (i == 0)
		set_result(1);
	else
		set_result(0);
}

static void bdos_filelength(void)
{
	struct fcb* fcb = find_fcb();
	struct file* f = file_open(&fcb->filename);

	int length = file_getrecordcount(f);
	fcb->r[0] = length;
	fcb->r[1] = length>>8;
	fcb->r[2] = length>>16;
}

static void bdos_getsetuser(void)
{
	if (get_e() == 0xff)
		set_result(0);
}

static void bdos_entry(uint8_t bdos_call)
{
	switch (bdos_call)
	{
		case  1: bdos_getchar();     return;
		case  2: bdos_putchar();     return;
		case  6: bdos_consoleio();   return;
		case  9: bdos_printstring(); return;
		case 10: bdos_readline();    return;
		case 11: bdos_consolestatus(); return;
		case 12: set_result(0x0022); return; // get CP/M version
		case 13: bdos_resetdisk();   return; // reset disk system
		case 14: bdos_selectdisk();  return; // select disk
		case 15: bdos_openfile();    return;
		case 16: bdos_closefile();   return;
		case 17: bdos_findfirst();   return;
		case 18: bdos_findnext();    return;
		case 19: bdos_deletefile();  return;
		case 20: bdos_readwritesequential(file_read);  return;
		case 21: bdos_readwritesequential(file_write); return;
		case 22: bdos_makefile();    return;
		case 23: bdos_renamefile();  return;
		case 24: set_result(0xffff); return; // get login vector
		case 25: bdos_getdisk();     return; // get current disk
		case 26: dma = get_de();     return; // set DMA
		case 27: set_result(0);      return; // get allocation vector
		case 29: set_result(0x0000); return; // get read-only vector
		case 31: set_result(0);      return; // get disk parameter block
		case 32: bdos_getsetuser();  return;
		case 33: bdos_readwriterandom(file_read);  return;
		case 34: bdos_readwriterandom(file_write); return;
		case 35: bdos_filelength(); return;
		case 40: bdos_readwriterandom(file_write); return;
		case 45:                     return; // set hardware error action
		case 108: exitcode = get_d(); return; // set exit code
	}

	showregs();
	fatal("unimplemented bdos entry %d", bdos_call);
}

void biosbdos_entry(int syscall)
{
	if (syscall == 0xff)
		bdos_entry(z80ex_get_reg(z80, regBC));
	else
		bios_entry(syscall);
}

