OUTPUT_FORMAT(binary)

SECTIONS {
    ram : {
        . = 0x0000;
        *(.text);

        . = 0xe800;
        CBASE = .;
        *(CCP);

        . = 0xf000;
        FBASE = .;
        *(BDOS);

        . = 0xfe00;
        BBASE = .;
        *(BIOS);
    }
}