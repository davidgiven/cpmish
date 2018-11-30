OUTPUT_FORMAT(binary)

MEMORY {
    supervisor   : ORIGIN = 0x0000, LENGTH = 8K
    splashscreen : ORIGIN = 0x2000, LENGTH = 1K
    autoprg      : ORIGIN = 0x4000, LENGTH = 512
}

SECTIONS {
    autoprg :      { *(AUTOPRG) } >autoprg
    splashscreen : { *(SPLASH) } >splashscreen
    supervisor :   { *(.text) } >supervisor
}