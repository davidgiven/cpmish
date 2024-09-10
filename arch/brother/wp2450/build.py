from build.ab import simplerule
from build.cpm import cpm_addresses, binslice, diskimage
from utils.build import unix2cpm
from third_party.zmac.build import zmac
from third_party.ld80.build import ld80


# Memory layout configuration -----------------------------------------------

# Configure the BIOS size here; this will then emit an addresses.lib file
# which contains the position of the BDOS and CCP.

(cbase, fbase, bbase) = cpm_addresses(name="addresses", bios_size=0x0F00)

# Generated tables ----------------------------------------------------------

# Font and keyboard table.

simplerule(
    name="keytab_inc",
    ins=["arch/brother/wp2450/utils+mkkeytab"],
    outs=["=keytab.inc"],
    commands=["{ins[0]} > {outs[0]}"],
)

simplerule(
    name="font_inc",
    ins=["arch/brother/wp2450/utils+fontconvert", "utils/6x7font.bdf"],
    outs=["=font.inc"],
    commands=["{ins[0]} {ins[1]} > {outs[0]}"],
)

# Libraries -----------------------------------------------------------------

# These may be used by both the BIOS proper and the bootstrap program.

zmac(
    name="tty_o",
    z180=True,
    src="./tty.z80",
    deps=[
        "include/cpm.lib",
        "include/cpmish.lib",
        "include/z180.lib",
        "arch/common/utils/tty.lib",
        "arch/common/utils/print.lib",
        "arch/brother/wp2450/include/wp2450.lib",
        ".+font_inc",
    ],
)

zmac(
    name="upd765_o",
    z180=True,
    src="./upd765.z80",
    deps=[
        "include/cpm.lib",
        "include/cpmish.lib",
        "include/z180.lib",
        "arch/common/utils/upd765.lib",
        "arch/common/utils/deblocker.lib",
        "arch/brother/wp2450/include/wp2450.lib",
    ],
)

zmac(
    name="keyboard_o",
    z180=True,
    src="./keyboard.z80",
    deps=[
        "include/cpm.lib",
        "include/cpmish.lib",
        "include/z180.lib",
        "arch/brother/wp2450/include/wp2450.lib",
        ".+keytab_inc",
    ],
)

# Bootstrapper --------------------------------------------------------------

# This is the .APL file which the Brother OS loads. It's responsible for
# remapping the memory, doing some device setup, and loading the BIOS into
# the top of memory.

zmac(
    name="boot_o",
    z180=True,
    src="./boot.z80",
    deps=[
        "include/cpm.lib",
        "include/cpmish.lib",
        "include/z180.lib",
        "arch/brother/wp2450/include/wp2450.lib",
        ".+addresses",
    ],
)

ld80(name="boot_img", address=0x5000, objs={0x5000: [".+boot_o", ".+upd765_o"]})

# BIOS ----------------------------------------------------------------------

# The CP/M BIOS itself.

zmac(
    name="bios_o",
    z180=True,
    src="./bios.z80",
    deps=[
        "include/cpm.lib",
        "include/cpmish.lib",
        "include/z180.lib",
        "arch/brother/wp2450/include/wp2450.lib",
        ".+addresses",
    ],
)

# This is a 64kB file containing the entire CP/M memory image.

ld80(
    name="memory_img",
    address=0,
    objs={
        cbase: ["third_party/zcpr1"],
        fbase: ["third_party/zsdos"],
        bbase: [
            ".+bios_o",
            ".+upd765_o",
            ".+tty_o",
            ".+keyboard_o",
        ],
    },
)

# We now chop it up to remove the BIOS and the combined CCP/BDOS (which
# go into the system track).

binslice(
    name="bios_img", src=".+memory_img", start=bbase, length=0x10000 - bbase
)

binslice(
    name="systemtrack_img",
    src=".+memory_img",
    start=cbase,
    length=bbase - cbase,
)

# FAT file system -----------------------------------------------------------

# Produces the FAT bit of the disk image.

zmac(
    name="bootfile_img",
    src="./fat.z80",
    deps=[
        ".+boot_img",
        ".+bios_img",
        ".+systemtrack_img",
    ],
    relocatable=False,
    z180=True,
)

# Disk image ----------------------------------------------------------------

# Assembles the bootable disk which you can actually run.

unix2cpm(name="readme", src="README.md")

diskimage(
    name="diskimage",
    format="brother-wp2450",
    bootfile=".+bootfile_img",
    map={
        "-readme.txt": ".+readme",
        "asm.com": "cpmtools+asm",
        "bbcbasic.com": "third_party/bbcbasic+bbcbasic_ADM3A",
        "camel80.com": "third_party/camelforth",
        "copy.com": "cpmtools+copy",
        "mkfs.com": "cpmtools+mkfs",
        "rawdisk.com": "cpmtools+rawdisk",
        "dump.com": "cpmtools+dump",
        "qe.com": "cpmtools+qe_BROTHER_WP2450DS",
        "ted.com": "third_party/ted+ted_WP2450DS",
        "z8e.com": "third_party/z8e+z8e_WP2450DS",
        "stat.com": "cpmtools+stat",
        "submit.com": "cpmtools+submit",
        "startrek.com": "third_party/startrek",
    },
)
