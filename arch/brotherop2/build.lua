include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
    name = "boot.o",
    srcs = { "./boot.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib"
    },
}

ld80 {
    name = "boot",
	address = 0x8000,
    srcs = {
		"-P8000",
		"+boot.o"
    }
}

zmac {
	name = "bios",
	srcs = { "./bios.z80" },
	deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib",
		"arch/common/utils/deblocker.lib"
    },
}

-- This is the bit which CP/M reloads on warm boot (well, some of it).
ld80 {
	name = "cpmfile",
	address = 0xe400,
	srcs = {
		"-Pe400", "third_party/zcpr1+zcpr",
		"-Pec00", "third_party/zsdos+zsdos",
		"-Pfa00", "+bios",
	}
}

-- Produces the FAT bit of the disk image.
zmac {
	name = "fat",
	srcs = { "./fat.z80" },
	deps = {
		"+boot",
		"+cpmfile"
	}
}

ld80 {
	name = "bootfile",
	srcs = { "+fat" }
}

diskimage {
	name = "diskimage",
	format = "brother-op2",
	bootfile = { "+bootfile" },
	map = {
		["dump.com"] = "cpmtools+dump",
		["stat.com"] = "cpmtools+stat",
		["asm.com"] = "cpmtools+asm",
		["copy.com"] = "cpmtools+copy",
		["submit.com"] = "cpmtools+submit",
		["bbcbasic.com"] = "third_party/bbcbasic+bbcbasic",
		["qe.com"] = "cpmtools+qe_BROTHEROP2",
	},
}

