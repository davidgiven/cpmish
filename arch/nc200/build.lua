include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

--- Memory layout configuration ------------------------------------------

-- Configure the BIOS size here; this will then emit an addresses.lib file
-- which contains the position of the BDOS and CCP.

local BIOS_SIZE = 0x0300
local BDOS_SIZE = 3584            -- fixed
local CCP_SIZE = 2048             -- fixed
local CPM_SIZE = BIOS_SIZE + BDOS_SIZE + CCP_SIZE
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
                "echo CPM_SIZE = "..CPM_SIZE.." >> %{outs[1]}",
                "echo BBASE = "..BBASE.." >> %{outs[1]}",
                "echo FBASE = "..FBASE.." >> %{outs[1]}",
                "echo CBASE = "..CBASE.." >> %{outs[1]}",
        }
}

--- Bootstrap ------------------------------------------------------------

-- The program the NC200 runs on startup.

zmac {
    name = "auto",
    srcs = { "./auto.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib"
    },
    relocatable = false
}

objectify {
    name = "auto_inc",
    srcs = { "+auto" }
}

--- FAT boot data --------------------------------------------------------

local bootfiles = filenamesof("./boot/*.z80")
for _, f in pairs(bootfiles) do
    local base = basename(f)
    zmac {
        name = base,
        srcs = { f },
        deps = {
            "include/*.lib",
            "./include/*.lib",
            "+auto_inc"
        }
    }
end

--- BIOS -----------------------------------------------------------------

-- The CP/M BIOS itself.

zmac {
    name = "bios",
    srcs = { "./bios.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"+addresses_lib",
    },
}

-- Builds the memory image. This is a 64kB file containing the entire CP/M
-- memory image, including the supervisor at the bottom.

ld80 {
    name = "memory_img",
	address = 0,
    srcs = {
        "-P0000", "+startup.z80",
        "-P000b", "+bpb1.z80",
        "-P0038", "+sirq.z80",
        "-P01fe", "+bootsig.z80",
        "-P020b", "+bpb2.z80",
        "-P0400", "+fat.z80",
        "arch/nc200/supervisor+variables",
        "-P1000", "+rootdir.z80",
        "arch/nc200/supervisor+supervisor",
        "-P1e00", "+relauto.z80",
        "-P"..string.format("%x", CBASE), "third_party/zcpr1+zcpr",
        "-P"..string.format("%x", FBASE), "third_party/zsdos+zsdos",
        "-P"..string.format("%x", BBASE), "+bios",
    }
}

-- We now chop it up to remove the supervisor and the CCP/BDOS/BIOS, which go into
-- the system tracks.

binslice {
	name = "cpm_img",
	src = { "+memory_img" },
	start = CBASE,
	length = CPM_SIZE
}

binslice {
	name = "supervisor_img",
	src = { "+memory_img" },
	start = 0,
	length = 9*1024
}

-- And combine them into the system track image.

normalrule {
    name = "systemtrack_img",
    ins = {
		"+supervisor_img",
		"+cpm_img",
	},
    outleaves = { "systemtrack.img" },
    commands = {
		"cat %{ins} > %{outs[1]}"
    }
}

unix2cpm {
    name = "readme",
    srcs = { "README.md" }
}

diskimage {
    name = "diskimage",
    format = "nc200cpm",
    bootfile = { "arch/nc200+systemtrack_img" },
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
        ["copy.com"] = "cpmtools+copy",
        ["submit.com"] = "cpmtools+submit",
        ["bbcbasic.com"] = "third_party/bbcbasic+bbcbasic_ADM3A",
        ["qe.com"] = "cpmtools+qe_NC200",
        ["flash.com"] = "arch/nc200/tools+flash",
        ["flipdisk.com"] = "arch/nc200/tools+flipdisk",
        ["mkfs.com"] = "cpmtools+mkfs",
        ["rawdisk.com"] = "cpmtools+rawdisk",
		["z8e.com"] = "third_party/z8e+z8e_NC200",
		["ted.com"] = "third_party/ted+ted_NC200",
        ["-readme.txt"] = "+readme",
    },
}
 
