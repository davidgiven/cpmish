#ifndef LIBCPM_H
#define LIBCPM_H

typedef struct
{
    uint8_t dr;
    uint8_t f[11];
    uint8_t ex;
    uint8_t s[2];
    uint8_t rc;
    uint8_t al[16];
    uint8_t cr;
    uint16_t r;
}
FCB;

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

extern FCB fcb;
extern FCB fcb2;

extern uint8_t cmdlinelen;
extern uint8_t cmdline[127];

extern uint16_t cpm_get_login_vector(void);
extern uint16_t cpm_get_rodisk_vector(void);
extern uint8_t* cpm_get_allocation_vector(void);
extern DPB* cpm_get_dpb(void);
extern uint8_t cpm_select_drive(uint8_t drive) __z88dk_fastcall;
extern uint8_t cpm_get_current_drive(void);
extern uint8_t cpm_write_protect_drive(void);

#endif
