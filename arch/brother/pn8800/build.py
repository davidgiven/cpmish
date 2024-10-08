from build.ab import simplerule
from build.cpm import cpm_addresses, binslice, diskimage
from utils.build import unix2cpm
from third_party.zmac.build import zmac
from third_party.ld80.build import ld80

# Memory layout configuration -----------------------------------------------

# Configure the BIOS size here; this will then emit an addresses.lib file
# which contains the position of the BDOS and CCP.

top_of_memory = 0x10000
(cbase, fbase, bbase) = cpm_addresses(
    name="addresses", top_of_memory=top_of_memory, bios_size=0x0900
)

# Generated tables ----------------------------------------------------------

simplerule(
    name="keytab_inc",
    ins=["arch/brother/pn8800/utils+mkkeytab"],
    outs=["=keytab.inc"],
    commands=["{ins[0]} > {outs[0]}"],
    label="MKKEYTAB",
)

# Object files --------------------------------------------------------------

BIOS_SRCS = [
    "boot",
    "hd63266",
    "bios",
    "floppy",
    "tty",
    "keyboard",
]

for name in BIOS_SRCS:
    zmac(
        name=name + "_o",
        z180=True,
        src=f"./{name}.z80",
        deps=[
            ".+addresses",
            ".+keytab_inc",
            "arch/brother/pn8800/include/pn8800.lib",
            "arch/common/utils/print.lib",
            "arch/common/utils/tty.lib",
            "include/cpm.lib",
            "include/cpmish.lib",
            "include/z180.lib",
        ],
    )

# Bootstrapper --------------------------------------------------------------

# This is the .APL file which the Brother OS loads. It's responsible for
# remapping the memory, doing some device setup, and loading the BIOS into
# the top of memory.

ld80(name="boot_img", address=0x8000, objs={0: [".+boot_o", ".+hd63266_o"]})

# BIOS ----------------------------------------------------------------------

# The CP/M BIOS itself.

# This is a 64kB file containing the entire CP/M memory image.

ld80(
    name="memory_img",
    address=0,
    objs={
        cbase: ["third_party/zcpr1"],
        fbase: ["third_party/zsdos"],
        bbase: [
            ".+bios_o",
            ".+hd63266_o",
            ".+tty_o",
            ".+floppy_o",
            ".+keyboard_o",
        ],
    },
)

# Currently only used for debugging.
binslice(
    name="bios_img", src=".+memory_img", start=bbase, length=0x10000 - bbase
)

binslice(
    name="systemtrack_img",
    src=".+memory_img",
    start=cbase,
    length=0x10000 - cbase,
)

# FAT file system -----------------------------------------------------------

# Produces the FAT bit of the disk image.

zmac(
    name="bootfile_img",
    src="./fat.z80",
    deps=[
        "./fat.z80",
        ".+boot_img",
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
    format="brother-powernote",
    bootfile=".+bootfile_img",
    size=1440 * 1024,
    map={
        "-readme.txt": ".+readme",
        "dump.com": "cpmtools+dump",
        "stat.com": "cpmtools+stat",
        "asm.com": "cpmtools+asm",
        "copy.com": "cpmtools+copy",
        "submit.com": "cpmtools+submit",
        "asm80.com": "third_party/dr/asm80",
        "camel80.com": "third_party/camelforth",
        "bbcbasic.com": "third_party/bbcbasic+bbcbasic_ADM3A",
        "mkfs.com": "cpmtools+mkfs",
        "rawdisk.com": "cpmtools+rawdisk",
        "qe.com": "cpmtools+qe_BROTHER_POWERNOTE",
        "z8e.com": "third_party/z8e+z8e_POWERNOTE",
        "ted.com": "third_party/ted+ted_POWERNOTE",
    },
)

# vim: ts=4 sw=4 et
