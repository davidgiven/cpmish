from build.ab import simplerule
from build.cpm import cpm_addresses, binslice, diskimage
from third_party.zmac.build import zmac
from third_party.ld80.build import ld80
from utils.build import unix2cpm

simplerule(
    name="font_inc",
    ins=["arch/brother/lw30/utils+fontconvert", "utils/6x7font.bdf"],
    outs=["=font.inc"],
    commands=["{ins[0]} {ins[1]} > {outs[0]}"],
    label="FONTCONVERT",
)

zmac(
    name="boot_o",
    z180=True,
    src="./boot.z80",
    deps=[
        "arch/brother/lw30/include/lw30.lib",
        "arch/common/utils/tty.lib",
        "include/cpmish.lib",
        "include/z180.lib",
        ".+font_inc",
    ],
)

ld80(name="boot", address=0x5000, objs={0x5000: [".+boot_o"]})

[
    zmac(
        name=n,
        z180=True,
        src=f"./{n}.z80",
        deps=[
            "arch/brother/lw30/include/lw30.lib",
            "arch/common/utils/print.lib",
            "arch/common/utils/tty.lib",
            "include/cpm.lib",
            "include/cpmish.lib",
            "include/z180.lib",
        ],
    )
    for n in ["bios", "floppy", "tty"]
]

# This is the bit which CP/M reloads on warm boot (well, some of it).
ld80(
    name="cpmfile",
    address=0x9300,
    objs={
        0x9300: ["third_party/zcpr1"],
        0x9B00: ["third_party/zsdos"],
        0xA900: [
            ".+bios",
            ".+floppy",
            ".+tty",
        ],
    },
)

# Produces the FAT bit of the disk image.
zmac(name="fat", z180=True, src="./fat.z80", deps=[".+boot", ".+cpmfile"])

ld80(name="bootfile", objs={0x0000: [".+fat"]})

unix2cpm(name="readme", src="README.md")

diskimage(
    name="diskimage",
    format="brother-op2",
    bootfile=".+bootfile",
    map={
        "-readme.txt": ".+readme",
        "dump.com": "cpmtools+dump",
        "stat.com": "cpmtools+stat",
        "asm.com": "cpmtools+asm",
        "copy.com": "cpmtools+copy",
        "submit.com": "cpmtools+submit",
        "rawdisk.com": "cpmtools+rawdisk",
        "bbcbasic.com": "third_party/bbcbasic+bbcbasic_ADM3A",
        "camel80.com": "third_party/camelforth",
        "qe.com": "cpmtools+qe_BROTHEROP2",
    },
)
