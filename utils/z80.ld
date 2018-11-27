OUTPUT_FORMAT(binary)

MEMORY {
    supervisor : ORIGIN = 0x0000, LENGTH = 8K
    autoprg    : ORIGIN = 0x4000, LENGTH = 512
}

SECTIONS {
    .text : { *(AUTOPRG) } >autoprg

    supervisor : { *(.text) *(.data) *(.bss) } >supervisor
}