include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

--- Memory layout configuration ---------------------------------------------

-- Configure the BIOS size here; this will then emit an addresses.lib file
-- which contains the position of the BDOS and CCP.

local BIOS_SIZE = 0x0500
local BDOS_SIZE = 3584            -- fixed
local CCP_SIZE = 2048             -- fixed
local BBASE = 0x9000 - BIOS_SIZE
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

--- Bootstrapper ------------------------------------------------------------

-- This is the .APL file which the Brother OS loads. It's responsible for
-- remapping the memory, doing some device setup, and loading the BIOS into
-- the top of memory.

zmac {
    name = "boot_img",
    srcs = { "./boot.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"+addresses_lib"
    },
	relocatable = false
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
		"arch/common/utils/deblocker.lib",
		"+addresses_lib",
    },
}

zmac {
    name = "tty_o",
    srcs = { "./tty.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib",
    },
}

zmac {
    name = "floppy_o",
    srcs = { "./floppy.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
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
		"+tty_o",
		"+floppy_o",
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
	name = "ccpbdos_img",
	src = { "+memory_img" },
	start = CBASE,
	length = BBASE - CBASE
}

--- Brother file system -----------------------------------------------------

-- Produces the fake Brother file system image

zmac {
	name = "bootfile_img",
	srcs = { "./fs.z80" },
	deps = {
		"+boot_img",
		"+bios_img",
		"+ccpbdos_img",
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
	format = "brother-wp1",
	bootfile = { "+bootfile_img" },
	map = {
        ["-readme.txt"] = "+readme",
	},
}



