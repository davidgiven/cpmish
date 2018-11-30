OUTPUT_FORMAT(binary)

MEMORY {
    supervisor   : ORIGIN = 0x0000, LENGTH = 8K
    splashscreen : ORIGIN = 0x2000, LENGTH = 1K
    autoprg      : ORIGIN = 0x4000, LENGTH = 512

    bios         : ORIGIN = 0xfe00, LENGTH = 0x0200
    bdos         : ORIGIN = 0xf000, LENGTH = 0x0e00
    ccp          : ORIGIN = 0xe800, LENGTH = 0x0800
}

SECTIONS {
    autoprg :      { *(AUTOPRG) } >autoprg
    splashscreen : { *(SPLASH) } >splashscreen
    ccp :          { *(CCP) } >ccp
    bdos :         { *(BDOS) } >bdos
    bios :         { *(BIOS) } >bios
    supervisor :   { *(.text) } >supervisor
}