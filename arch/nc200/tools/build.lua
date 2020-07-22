include "cpmtools/libcuss/build.lua"

ackprogram {
    name = "flash",
    srcs = {
        "./flash.c",
        "./pcmcia-tools.s"
    },
}

zmac {
	name = "flipdisk_o",
	srcs = { "./flipdisk.z80" },
	deps = { "arch/nc200+addresses_lib" }
}

ld80 {
	name = "flipdisk",
	address = 0x100,
	srcs = {
		"-P0100",
		"+flipdisk_o"
	}
}

