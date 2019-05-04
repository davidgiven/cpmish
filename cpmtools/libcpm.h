#ifndef LIBCPM_H
#define LIBCPM_H

typedef struct
{
    uint8_t dr;
    uint8_t f[11];
    uint8_t ex;
    uint8_t s[2];
    uint8_t rc;
    uint8_t d[16];
    uint8_t cr;
    uint16_t r;
    uint8_t r2;
}
FCB;

typedef struct
{
    uint8_t us;
    uint8_t f[11];
    uint8_t ex;
    uint8_t s[2];
    uint8_t rc;
    uint8_t al[16];
}
DIRE;

typedef struct
{
    uint16_t spt; /* number of 128-byte sectors per track */
    uint8_t bsh;  /* block shift; 3=1kB, 4=2kB, 5=4kB etc */
    uint8_t blm;  /* block mask; 0x07=1kB, 0x0f=2kB, 0x1f=4k etc */
    uint8_t exm;  /* extent mask */
    uint16_t dsm; /* maximum block number */
    uint16_t drm; /* maximum directory entry number */
    uint16_t al;  /* directory allocation bitmap */
    uint16_t cks; /* checksum vector size */
    uint16_t off; /* number of reserved tracks */
}
DPB;

extern FCB cpm_fcb;
extern uint8_t cpm_iobyte;

extern uint8_t cpm_cmdlinelen;
extern uint8_t cpm_cmdline[127];

extern uint16_t cpm_get_login_vector(void);
extern uint16_t cpm_get_rodisk_vector(void);
extern uint8_t* cpm_get_allocation_vector(void);
extern DPB* cpm_get_dpb(void);
extern uint8_t cpm_select_drive(uint8_t drive) __z88dk_fastcall;
extern uint8_t cpm_get_current_drive(void);
extern uint8_t cpm_get_current_user(void);
extern uint8_t cpm_get_console_status(void);
extern uint8_t cpm_write_protect_drive(void);
extern void cpm_set_dma(void* dma) __z88dk_fastcall;
extern uint8_t cpm_findfirst(FCB* fcb) __z88dk_fastcall;
extern uint8_t cpm_findnext(FCB* fcb) __z88dk_fastcall;
extern uint8_t cpm_open_file(FCB* fcb) __z88dk_fastcall;
extern uint8_t cpm_read_sequential(FCB* fcb) __z88dk_fastcall;

#endif
