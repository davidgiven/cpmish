package(default_visibility = ["//visibility:public"])

load("//build:cpm.bzl", "cpm_addresses", "binslice", "diskimage")
load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")
load("//utils:build.bzl", "unix2cpm")

# Memory layout configuration -----------------------------------------------

# Configure the BIOS size here; this will then emit an addresses.lib file
# which contains the position of the BDOS and CCP.

top_of_memory = 0x10000
(cbase, fbase, bbase) = cpm_addresses(
    name = "addresses",
    top_of_memory = top_of_memory,
    bios_size = 0x0900
)

# Generated tables ----------------------------------------------------------

genrule(
    name = "keytab_inc",
	tools = [ "//arch/brotherpowernote/utils:mkkeytab" ],
    outs = [ "keytab.inc" ],
	cmd = "$(location //arch/brotherpowernote/utils:mkkeytab) > $@"
)

# Object files --------------------------------------------------------------

BIOS_SRCS = [
    "boot",
    "floppy",
    "tty",
    "keyboard",
]

[
    zmac(
        name = name + "_o",
        z180 = True,
        srcs = [
            name + ".z80",
            "//include:cpm.lib",
            "//include:cpmish.lib",
            "//include:z180.lib",
            ":keytab_inc",
            "//arch/brotherpowernote/include:brotherpowernote.lib",
            "//arch/common/utils:tty.lib",
            "//arch/common/utils:print.lib",
            ":addresses",
        ]
    )
    for name in BIOS_SRCS
]

# Bootstrapper --------------------------------------------------------------

# This is the .APL file which the Brother OS loads. It's responsible for
# remapping the memory, doing some device setup, and loading the BIOS into
# the top of memory.

ld80(
    name = "boot_img",
    address = 0x8000,
    objs = {
        0: [ ":boot_o", ]
    }
)

# BIOS ----------------------------------------------------------------------

# The CP/M BIOS itself.

zmac(
    name = "bios_o",
    z180 = True,
    srcs = [
        "bios.z80",
        "//include:cpm.lib",
        "//include:cpmish.lib",
        "//include:z180.lib",
        "//arch/brotherpowernote/include:brotherpowernote.lib",
        ":addresses",
    ],
)

# This is a 64kB file containing the entire CP/M memory image.

ld80(
    name = "memory_img",
    address = 0,
    objs = {
        cbase: [ "//third_party/zcpr1" ],
        fbase: [ "//third_party/zsdos" ],
        bbase: [
            ":bios_o",
            ":tty_o",
            ":floppy_o",
            ":keyboard_o",
        ]
    }
)

# Currently only used for debugging.
binslice(
    name = "bios_img",
    src = ":memory_img",
    start = bbase,
    length = 0x10000 - bbase
)

binslice(
    name = "systemtrack_img",
    src = ":memory_img",
    start = cbase,
    length = 0x10000 - cbase
)

# FAT file system -----------------------------------------------------------

# Produces the FAT bit of the disk image.

zmac(
	name = "bootfile_img",
	srcs = [
		"fat.z80",
		":boot_img",
        ":bios_img",
        ":systemtrack_img",
	],
	relocatable = False,
	z180 = True,
)

# Disk image ----------------------------------------------------------------

# Assembles the bootable disk which you can actually run.

diskimage(
	name = "diskimage",
	format = "brother-powernote",
	bootfile = ":bootfile_img",
	map = {
        "dump.com": "//cpmtools:dump",
        "stat.com": "//cpmtools:stat",
        "asm.com": "//cpmtools:asm",
        "copy.com": "//cpmtools:copy",
        "submit.com": "//cpmtools:submit",
        "asm80.com": "//third_party/dr/asm80",
        "camel80.com": "//third_party/camelforth",
        "bbcbasic.com": "//third_party/bbcbasic:bbcbasic_ADM3A",
        "mkfs.com": "//cpmtools:mkfs",
        "rawdisk.com": "//cpmtools:rawdisk",
        "qe.com": "//cpmtools:qe_BROTHER_POWERNOTE",
        "z8e.com": "//third_party/z8e:z8e_POWERNOTE",
        "ted.com": "//third_party/ted:ted_POWERNOTE",
	},
)

# vim: ts=4 sw=4 et
        
