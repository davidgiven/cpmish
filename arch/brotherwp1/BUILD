package(default_visibility = ["//visibility:public"])

load("//build:cpm.bzl", "cpm_addresses", "binslice", "diskimage")
load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")
load("//utils:build.bzl", "unix2cpm")

# Memory layout configuration -----------------------------------------------

# Configure the BIOS size here; this will then emit an addresses.lib file
# which contains the position of the BDOS and CCP.

top_of_memory = 0x9000
(cbase, fbase, bbase) = cpm_addresses(
    name = "addresses",
    top_of_memory = top_of_memory,
    bios_size = 0x0a00
)

# Bootstrapper --------------------------------------------------------------

# This is the .APL file which the Brother OS loads. It's responsible for
# remapping the memory, doing some device setup, and loading the BIOS into
# the top of memory.

zmac(
    name = "boot_img",
    relocatable = False,
	z180 = True,
    srcs = [
        "boot.z80",
        "//include:z180.lib",
        ":addresses",
    ],
)

# BIOS ----------------------------------------------------------------------

BIOS_SRCS = [
    "bios",
    "floppy",
    "keyboard",
    "tty",
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
            "//arch/brotherwp1/include:brotherwp1.lib",
            "//arch/common/utils:tty.lib",
            "//arch/common/utils:print.lib",
            ":addresses",
            ":keytab",
        ]
    )
    for name in BIOS_SRCS
]

genrule(
    name = "keytab",
    tools = [ "//arch/brotherwp1/utils:mkkeytab" ],
    srcs = [],
    outs = [ "keytab.inc" ],
    cmd = "$(location //arch/brotherwp1/utils:mkkeytab) > $@"
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

# We now chop it up to remove the BIOS and the combined CCP/BDOS (which
# go into the system track).

binslice(
    name = "bios_img",
    src = ":memory_img",
    start = bbase,
    length = top_of_memory - bbase
)

binslice(
    name = "ccpbdos_img",
    src = ":memory_img",
    start = cbase,
    length = bbase - cbase
)

# Brother file system -------------------------------------------------------

# Produces the fake Brother file system image

zmac(
	name = "bootfile_img",
	z180 = True,
	srcs = [
        "fs.z80",
		":boot_img",
		":bios_img",
		":ccpbdos_img",
	],
	relocatable = False
)

# Disk image ----------------------------------------------------------------

# Assembles the bootable disk which you can actually run.

unix2cpm(
    name = "readme",
    src = "README.md"
)

diskimage(
    name = "diskimage",
    format = "brother-wp1",
    bootfile = ":bootfile_img",
    map = {
        "-readme.txt": ":readme",
        "dump.com": "//cpmtools:dump",
        "stat.com": "//cpmtools:stat",
        "asm.com": "//cpmtools:asm",
        "copy.com": "//cpmtools:copy",
        "submit.com": "//cpmtools:submit",
        "bbcbasic.com": "//third_party/bbcbasic:bbcbasic_ADM3A",
        "camel80.com": "//third_party/camelforth",
        "qe.com": "//cpmtools:qe_BROTHER_WP1",
    },
)

# vim: ts=4 sw=4 et
        
