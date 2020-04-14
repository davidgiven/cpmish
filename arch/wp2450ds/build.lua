include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

normalrule {
    name = "font_inc",
    ins = {
        "arch/wp2450ds/utils+fontconvert",
        "utils+6x7font",
    },
    outleaves = { "font.inc" },
    commands = {
        "%{ins} > %{outs}"
    }
}

zmac {
    name = "tty_o",
    srcs = { "./tty.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib",
		"+font_inc"
    },
}

zmac {
    name = "upd765_o",
    srcs = { "./upd765.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/upd765.lib",
		"arch/common/utils/deblocker.lib",
    },
}

zmac {
    name = "bios_o",
    srcs = { "./bios.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib",
		"arch/common/utils/upd765.lib",
		"arch/common/utils/deblocker.lib",
		"+font_inc"
    },
}

-- This is the bit which CP/M reloads on warm boot (well, some of it).
ld80 {
	name = "cpmfile_img",
	address = 0x9300,
	srcs = {
		"-P9300", "third_party/zcpr1+zcpr",
		"-P9b00", "third_party/zsdos+zsdos",
		"-Pa900",
		"+bios_o",
		"+upd765_o",
		"+tty_o",
	}
}

zmac {
    name = "boot_o",
    srcs = { "./boot.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/upd765.lib",
		"arch/common/utils/deblocker.lib",
    },
}

ld80 {
    name = "boot_img",
	address = 0x5000,
    srcs = {
		"-P5000",
		"+boot_o",
		"+upd765_o",
    }
}

-- Produces the FAT bit of the disk image.
zmac {
	name = "fat_o",
	srcs = { "./fat.z80" },
	deps = {
		"+boot_img",
		"+cpmfile_img"
	}
}

ld80 {
	name = "bootfile_img",
	srcs = { "+fat_o" }
}

unix2cpm {
    name = "readme",
    srcs = { "README.md" }
}

diskimage {
	name = "diskimage",
	format = "brother-wp2450ds",
	bootfile = { "+bootfile_img" },
	map = {
		["dump.com"] = "cpmtools+dump",
		["stat.com"] = "cpmtools+stat",
		["asm.com"] = "cpmtools+asm",
		["copy.com"] = "cpmtools+copy",
		["submit.com"] = "cpmtools+submit",
		["bbcbasic.com"] = "third_party/bbcbasic+bbcbasic",
		["mkfs.com"] = "cpmtools+mkfs",
		["rawdisk.com"] = "cpmtools+rawdisk",
        ["-readme.txt"] = "+readme",
	},
}


