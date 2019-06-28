/* dump © 2019 David Given
 * This program is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 * 
 * A traditional CP/M stat.com clone. This implements the unbelievably baroque
 * and poorly-thought-out stat user interface as closely as possible, because
 * traditions.
 */

#include <cpm.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define FCB_COUNT 512

uint8_t accumulator[4];
uint8_t ibp = 0;
DPB* dpb;
uint16_t bpb; /* bytes per block */

struct fe
{
    uint8_t filename[11];
    uint8_t extents;
    uint16_t blocks;
    uint16_t records;
};

struct fe files[FCB_COUNT];
struct fe* findex[FCB_COUNT]; /* pointers to above (for sorting) */

const char logical_device_names[] = "CON:RDR:PUN:LST:DEV:VAL:USR:DSK:";
enum { CON = 1, RDR, PUN, LST, DEV, VAL, USR, DSK };

const char physical_device_names[] =
    /* CON: */ "TTY:CRT:BAT:UC1:"
    /* RDR: */ "TTY:PTR:UR1:UR2:"
    /* PUN: */ "TTY:PTP:UP1:UP2:"
    /* LST: */ "TTY:CRT:LPT:UL1:";

const FCB wildcard_fcb_template =
{
    /* dr */ '?',
    /* f  */ "???????????"
};

void print(const char* s)
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        cpm_conout(b);
    }
}

void crlf(void)
{
    print("\r\n");
}

void printx(const char* s)
{
    print(s);
    crlf();
}

/* 
 * Prints a 16-bit decimal number with optional left padding and configurable
 * precision. *.
 */
void printip(uint16_t v, bool pad, uint16_t precision)
{
    bool zerosup = true;
    while (precision)
    {
        uint8_t d = v / precision;
        v %= precision;
        precision /= 10;
        if (precision && zerosup && !d)
        {
            if (pad)
                cpm_conout(' ');
        }
        else
        {
            zerosup = false;
            cpm_conout('0' + d);
        }
    }
}

void printi(uint16_t v)
{
    printip(v, false, 10000);
}

/* Compares the accumulator with an array of uint8_t[4] words. Returns the
 * matching index plus one or 0. */
uint8_t compare_accumulator(const char* list, uint8_t length)
{
    uint8_t match = 1;
    while (length--)
    {
        bool m = true;
        uint8_t j;

        for (j=0; j<4; j++)
        {
            if (*list++ != accumulator[j])
                m = false;
        }
        if (m)
        {
            /* Matched! */
            return match;
        }
        match++;
    }
    return 0;
}

void select_disk(uint8_t d)
{
    cpm_select_drive(d);
    dpb = cpm_get_dpb();
    bpb = 1<<(dpb->bsh+7);
}

void select_fcb_disk(void)
{
    uint8_t drive = cpm_fcb.dr ? (cpm_fcb.dr-1) : cpm_get_current_drive();
    select_disk(drive);
}

uint16_t count_space(void)
{
    uint8_t* alloca = cpm_get_allocation_vector();
    uint16_t blocks = 0;
    uint16_t i;

    for (i=0; i<=U16(dpb->dsm); i++)
    {
        bool bit = alloca[i >> 3] & (0x80 >> (i & 7));
        if (!bit)
            blocks++;
    }
    return blocks;
}

void print_free_space(void)
{
    uint16_t blocks = count_space();
    printi(blocks << (dpb->bsh - 3));
    print("/");
    printi((U16(dpb->dsm)+1) << (dpb->bsh - 3));
    print("kB");
}

/* Show status of all drives. */
void print_drive_status(void)
{
    uint16_t login = cpm_get_login_vector();
    uint16_t rodisk = cpm_get_readonly_vector();

    uint8_t d = 0;
    while (login)
    {
        if (login & 1)
        {
            select_disk(d);
            cpm_conout('A' + d);
            print(": R/");
            cpm_conout((rodisk & 1) ? 'O' : 'W');
            print(", space: ");
            print_free_space();
            crlf();
        }

        login >>= 1;
        rodisk >>= 1;
        d++;
    }
}

/* Reads the next input token into the 4-byte accumulator. */
void scan(void)
{
    uint8_t obp;

    /* Skip whitespace. */

    while ((ibp != cpm_cmdlinelen) && (cpm_cmdline[ibp] == ' '))
        ibp++;

    obp = 0;
    memset(accumulator, ' ', sizeof(accumulator));
    while (obp != 4)
    {
        uint8_t b;

        if (ibp == cpm_cmdlinelen)
            return;

        b = cpm_cmdline[ibp++];
        accumulator[obp++] = b;
        switch (b)
        {
            case 0:
            case ',':
            case ':':
            case '*':
            case '.':
            case '>':
            case '<':
            case '=':
                return;
        }
    }
}

void printipadded(uint16_t value)
{
    printip(value, true, 10000);
}

void get_detailed_drive_status(void)
{
    print("    ");
    cpm_conout(cpm_get_current_drive() + 'A');
    cpm_conout(':');
    printx(" Drive Characteristics");

    {
        uint16_t rpb = 1<<dpb->bsh;
        uint16_t rpd = (U16(dpb->dsm)+1) * rpb;
        if ((rpd == 0) && (rpb != 0))
            print("65536");
        else
            printipadded(rpd);
        printx(": 128 byte record capacity");
    }

    printipadded(count_space());
    printx(": kilobyte drive capacity");

    printipadded(U16(dpb->drm)+1);
    printx(": 32 byte directory entries");

    printipadded(U16(dpb->cks));
    printx(": checked directory entries");

    printipadded((dpb->exm+1)*128);
    printx(": records per extent");

    printipadded(U16(dpb->spt));
    printx(": sectors per track");

    printipadded(U16(dpb->off));
    printx(": reserved tracks");
}

int index_sort_cb(const void* left, const void* right)
{
    const struct fe** leftp = (const struct fe**) left;
    const struct fe** rightp = (const struct fe**) right;
    return memcmp((*leftp)->filename, (*rightp)->filename, 11);
}

void print_filename(uint8_t* filename)
{
    uint8_t i;

    for (i=0; i<11; i++)
    {
        uint8_t b = *filename++ & 0x7f;
        if (b != ' ')
        {
            if (i == 8)
                cpm_conout('.');
            cpm_conout(b);
        }
    }
}

void file_manipulation(void)
{
    uint8_t command;
    uint16_t count;
    uint8_t r;

    /* Options are passed as the second word on the command line, which the
     * CPP parses as a filename and writes into cpm_fcb2. There will now be
     * a short pause for me to be ill. */

    const static char command_names[] = "$S  $R/O$R/W$SYS$DIR";
    enum { LIST = 0, LIST_WITH_SIZE, SET_RO, SET_RW, SET_SYS, SET_DIR };
    memcpy(accumulator, cpm_fcb2.f, 4);
    command = compare_accumulator(command_names, sizeof(command_names)/4);

    select_fcb_disk();
    cpm_fcb.ex = '?'; /* find all extents, not just the first */
    count = 0;
    r = cpm_findfirst(&cpm_fcb);
    while (r != 0xff)
    {
        DIRE* de = (DIRE*)0x80 + r;
        struct fe* fe = files;
        uint8_t j;

        /* Try to find the file in the array. */

        uint16_t i = count;
        while (i)
        {
            if (memcmp(fe->filename, de->f, 11) == 0)
                break;
            fe++;
            i--;
        }

        if (!i)
        {
            /* Not found --- add it. */

            memset(fe, 0, sizeof(*fe));
            memcpy(fe->filename, de->f, 11);
            findex[count] = fe;

            count++;
            if (count == FCB_COUNT)
            {
                printx("Too many files!");
                return;
            }
        }

        fe->extents++;
        fe->records += de->rc + (de->ex & dpb->exm)*128;
        if (U16(dpb->dsm) < 256)
        {
            /* 8-bit allocation map. */
            for (j=0; j<16; j++)
            {
                if (de->al[j])
                    fe->blocks++;
            }
        }
        else
        {
            /* 16-bit allocation map. */
            for (j=0; j<16; j+=2)
            {
                if (U16(de->al + j))
                    fe->blocks++;
            }
        }

        r = cpm_findnext(&cpm_fcb);
    }

    switch (command)
    {
        case LIST:
        case LIST_WITH_SIZE:
        {
            uint8_t current_drive;
            struct fe** fep;

            #if !defined SDCC || (SDCC >= 380)
                qsort(findex, count, sizeof(void*), index_sort_cb);
            #endif

            current_drive = 'A' + cpm_get_current_drive();
            if (command == LIST_WITH_SIZE)
                print("  Size");
            printx(" Recs   Bytes  Ext Acc");
            fep = findex;
            while (count--)
            {
                struct fe* f = *fep++;
                if (command == LIST_WITH_SIZE)
                {
                    memset(&cpm_fcb, 0, sizeof(FCB));
                    memcpy(cpm_fcb.f, f->filename, 11);
                    cpm_seek_to_end(&cpm_fcb);
                    if (cpm_fcb.r[2])
                        print("65536");
                    else
                        printipadded(U16(cpm_fcb.r));
                    cpm_conout(' ');
                }
                printipadded(f->records);
                cpm_conout(' ');
                printipadded(f->blocks << (dpb->bsh - 3));
                print("kB ");
                printip(f->extents, true, 1000);
                print((f->filename[8] & 0x80) ? " R/O " : " R/W ");
                cpm_conout(current_drive);
                cpm_conout(':');
                if (f->filename[9] & 0x80)
                    cpm_conout('(');
                print_filename(f->filename);
                if (f->filename[9] & 0x80)
                    cpm_conout(')');
                crlf();
                if (cpm_const())
                    return;
            }
            print("Bytes remaining on ");
            cpm_conout(current_drive);
            print(": ");
            print_free_space();
            crlf();
            break;
        }

        case SET_RO:
        case SET_RW:
        case SET_SYS:
        case SET_DIR:
        {
            uint8_t attrbyte = ((command == SET_RO) || (command == SET_RW)) ? 8 : 9;
            uint8_t attrflag = ((command == SET_RO) || (command == SET_SYS)) ? 0x80 : 0x00;

            struct fe* fe = files;
            while (count--)
            {
                const char* p;

                print_filename(fe->filename);
                memset(&cpm_fcb, 0, sizeof(FCB));
                memcpy(cpm_fcb.f, fe->filename, 11);
                cpm_fcb.f[attrbyte] = (cpm_fcb.f[attrbyte] & 0x7f) | attrflag;
                cpm_set_file_attributes(&cpm_fcb);

                print(" set to ");
                p = &command_names[(command-1)*4] + 1;
                cpm_conout(*p++);
                cpm_conout(*p++);
                cpm_conout(*p);
                crlf();
                if (cpm_const())
                    return;

                fe++;
            }

            break;
        }
    }
}

/* Handles the A:=R/O and A: DSK: cases. */
void set_drive_status(void)
{
    scan(); /* drive is already in fcb.dr */
    scan(); /* read = */
    if (accumulator[0] == '=')
    {
        scan(); /* get assignment */
        if (compare_accumulator("R/O ", 1))
        {
            select_fcb_disk();
            cpm_write_protect_drive();
        }
        else
            printx("Invalid disk assignment (only R/O is supported)");
    }
    else
    {
        /* Not a disk assignment --- the user must be trying to stat a file. */

        select_fcb_disk();
        if (compare_accumulator(logical_device_names, 8) == DSK)
            get_detailed_drive_status();
        else
            file_manipulation();
    }
}

void print_device_name(const char* p)
{
    for (;;)
    {
        uint8_t c = *p++;
        cpm_conout(c);
        if (c == ':')
            break;
    }
}

bool change_device_assignment(uint8_t logical)
{
    uint8_t physical;
    uint8_t b;

    scan();
    if (accumulator[0] != '=')
    {
        print("Bad delimiter");
        return true;
    }

    scan();
    physical = compare_accumulator(&physical_device_names[logical*16], 4) - 1;
    if (physical == 0xff)
    {
        printx("Invalid assignment");
        return true;
    }

    b = 3;
    while (logical--)
    {
        b <<= 2;
        physical <<= 2;
    }
    cpm_iobyte &= ~b;
    cpm_iobyte |= physical;
    return false;
}

void show_device_assignments(void)
{
    uint8_t b = cpm_iobyte;
    const char* lp = logical_device_names;
    const char* pp = physical_device_names;
    uint8_t i;

    for (i=0; i<4; i++)
    {
        print_device_name(lp);
        print(" is ");
        print_device_name(pp + (b & 3)*4);
        crlf();
        b >>= 2;
        lp += 4;
        pp += 16;
    }
}

void show_help(void)
{
    const char* lp;
    const char* pp;
    uint8_t i;
    uint8_t j;

    printx(
        "Set disk to read only:  stat d:=R/O\r\n"
        "Set file attributes:    stat d:filename.typ $R/O / $R/W / $SYS / $DIR\r\n"
        "Get file attributes:    stat d:filename.typ [ $S ]\r\n"
        "Show disk info:         stat DSK: / d: DSK:\r\n"
        "Show user number usage: stat USR:\r\n"
        "Show device mapping:    stat DEV:"
    );

    lp = logical_device_names;
    pp = physical_device_names;
    for (i=0; i<4; i++)
    {
        print("Set device mapping:     stat ");
        print_device_name(lp);
        lp += 4;
        cpm_conout('=');

        for (j=0; j<4; j++)
        {
            if (j)
                print(" / ");
            print_device_name(pp);
            pp += 4;
        }
        crlf();
    }
}

void show_user_numbers(void)
{
    static uint8_t users[32];
    static FCB wildcard_fcb;
    uint8_t r;
    uint8_t i;
    DIRE* data;

    print("Active user: ");
    printi(cpm_get_set_user(0xff));
    crlf();
    print("Active files:");

    data = (DIRE*) files; /* horrible, but it's unused and big enough */
    cpm_set_dma(data);
    memset(users, 0, sizeof(users));

    memcpy(&wildcard_fcb, &wildcard_fcb_template, sizeof(FCB));
    r = cpm_findfirst(&wildcard_fcb);
    while (r != 0xff)
    {
        DIRE* found = &data[r];
        /* On disk, dr contains the user number */
        if (found->us != 0xe5)
            users[found->us & 0x1f] = 1;
        r = cpm_findnext(&wildcard_fcb);
    }

    for (i=0; i<32; i++)
    {
        if (users[i])
        {
            cpm_conout(' ');
            printi(i);
        }
    }

    crlf();
}

/* Handle device assignment, querying, and miscellaneous other things */
bool device_manipulation(void)
{
    uint8_t items = 0;
    for (;;)
    {
        uint8_t i;

        scan();
        i = compare_accumulator(logical_device_names, 8);
        if (!i)
            return items;
        items++;

        switch (i)
        {
            default:
                if (change_device_assignment(i-1))
                    return true;
                break;

            case DEV:
                show_device_assignments();
                break;

            case VAL:
                show_help();
                break;

            case USR: /* Show user number usage on the current disk */
                select_fcb_disk();
                show_user_numbers();
                break;

            case DSK:
                select_fcb_disk();
                get_detailed_drive_status();
                break;
        }

        scan();
        if (accumulator[0] == ' ')
            return true;
        if (accumulator[0] != ',')
            goto bad_delimiter;
    }

bad_delimiter:
    print("Bad delimiter '");
    printi(accumulator[0]);
    printx("'");
    return true;
}

void main(void)
{
    if (!cpm_fcb.dr && (cpm_fcb.f[0] == ' '))
        print_drive_status();
    else if (cpm_fcb.dr)
        set_drive_status();
    else if (!device_manipulation())
        file_manipulation();
}
