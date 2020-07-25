include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

--- Memory layout configuration ---------------------------------------------

-- Configure the BIOS size here; this will then emit an addresses.lib file
-- which contains the position of the BDOS and CCP.

local BIOS_SIZE = 0x0f00
local BDOS_SIZE = 3584            -- fixed
local CCP_SIZE = 2048             -- fixed
local BBASE = 0x10000 - BIOS_SIZE
local FBASE = BBASE - BDOS_SIZE
local CBASE = FBASE - CCP_SIZE

normalrule {
	name = "addresses_lib",
	ins = {},
	outleaves = { "addresses.lib" },
	commands = {
		"echo BIOS_SIZE = "..BIOS_SIZE.." > %{outs[1]}",
		"echo BDOS_SIZE = "..BDOS_SIZE.." >> %{outs[1]}",
		"echo CCP_SIZE = "..CCP_SIZE.." >> %{outs[1]}",
		"echo BBASE = "..BBASE.." >> %{outs[1]}",
		"echo FBASE = "..FBASE.." >> %{outs[1]}",
		"echo CBASE = "..CBASE.." >> %{outs[1]}",
	}
}

--- Generated tables --------------------------------------------------------

-- Font and keyboard table.

normalrule {
    name = "font_inc",
    ins = {
        "arch/wp2450ds/utils+fontconvert",
        "utils/6x7font.bdf",
    },
    outleaves = { "font.inc" },
    commands = {
        "%{ins} > %{outs}"
    }
}

normalrule {
    name = "keytab_inc",
    ins = { "arch/wp2450ds/utils+mkkeytab" },
    outleaves = { "keytab.inc" },
    commands = {
        "%{ins} > %{outs}"
    }
}

--- Libraries ---------------------------------------------------------------

-- These may be used by both the BIOS proper and the bootstrap program.

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
    name = "keyboard_o",
    srcs = { "./keyboard.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"+keytab_inc",
    },
}

--- Bootstrapper ------------------------------------------------------------

-- This is the .APL file which the Brother OS loads. It's responsible for
-- remapping the memory, doing some device setup, and loading the BIOS into
-- the top of memory.

zmac {
    name = "boot_o",
    srcs = { "./boot.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/upd765.lib",
		"arch/common/utils/deblocker.lib",
		"+addresses_lib"
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

--- BIOS --------------------------------------------------------------------

-- The CP/M BIOS itself.

zmac {
    name = "bios_o",
    srcs = { "./bios.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib",
		"arch/common/utils/upd765.lib",
		"arch/common/utils/deblocker.lib",
		"+font_inc",
		"+addresses_lib",
    },
}

-- This is a 64kB file containing the entire CP/M memory image.

ld80 {
	name = "memory_img",
	address = 0,
	srcs = {
		"-P"..string.format("%x", CBASE), "third_party/zcpr1+zcpr",
		"-P"..string.format("%x", FBASE), "third_party/zsdos+zsdos",
		"-P"..string.format("%x", BBASE),
		"+bios_o",
		"+upd765_o",
		"+tty_o",
		"+keyboard_o",
	}
}

-- We now chop it up to remove the BIOS and the combined CCP/BDOS (which
-- go into the system track).

binslice {
	name = "bios_img",
	src = { "+memory_img" },
	start = BBASE,
	length = BIOS_SIZE
}

binslice {
	name = "systemtrack_img",
	src = { "+memory_img" },
	start = CBASE,
	length = BBASE - CBASE
}

--- FAT file system ---------------------------------------------------------

-- Produces the FAT bit of the disk image.

zmac {
	name = "bootfile_img",
	srcs = { "./fat.z80" },
	deps = {
		"+boot_img",
		"+bios_img",
		"+systemtrack_img"
	},
	relocatable = false
}

--- Disk image --------------------------------------------------------------

-- Assembles the bootable disk which you can actually run.

unix2cpm {
    name = "readme",
    srcs = { "README.md" }
}

diskimage {
	name = "diskimage",
	format = "brother-wp2450ds",
	bootfile = { "+bootfile_img" },
	map = {
		["asm.com"] = "cpmtools+asm",
		["copy.com"] = "cpmtools+copy",
		["dump.com"] = "cpmtools+dump",
		["mkfs.com"] = "cpmtools+mkfs",
		["rawdisk.com"] = "cpmtools+rawdisk",
		["stat.com"] = "cpmtools+stat",
		["submit.com"] = "cpmtools+submit",
        ["-readme.txt"] = "+readme",
		["bbcbasic.com"] = "third_party/bbcbasic+bbcbasic_ADM3A",
        ["qe.com"] = "cpmtools+qe_BROTHER_WP2450DS",
		["z8e.com"] = "third_party/z8e+z8e_WP2450DS",
		["ted.com"] = "third_party/ted+ted_WP2450DS",
	},
}


