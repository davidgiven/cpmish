from build.ab import simplerule
from build.cpm import cpm_addresses, diskimage
from utils.build import unix2cpm, objectify
from third_party.zmac.build import zmac
from third_party.ld80.build import ld80
from glob import glob

# Memory layout configuration -----------------------------------------------

# Configure the BIOS size here; this will then emit an addresses.lib file
# which contains the position of the BDOS and CCP.

(cbase, fbase, bbase) = cpm_addresses(name="addresses", bios_size=0x0300)

# Bootstrap -----------------------------------------------------------------

# The program the NC200 runs on startup.

zmac(
    name="auto",
    src="./auto.z80",
    deps=[
        "arch/nc200/include/nc200.lib",
    ],
    relocatable=False,
)

objectify(
    name="auto_inc",
    src=".+auto",
)

# FAT boot data -------------------------------------------------------------

for f in [
    "bootsig",
    "bpb1",
    "bpb2",
    "fat",
    "relauto",
    "rootdir",
    "sirq",
    "startup",
]:
    zmac(
        name=f"boot/{f}",
        src=f"./boot/{f}.z80",
        deps=[
            ".+auto_inc",
        ],
    )

# BIOS ----------------------------------------------------------------------

# The CP/M BIOS itself.

zmac(
    name="bios",
    src="./bios.z80",
    deps=[
        "arch/nc200/include/nc200.lib",
        "arch/nc200/include/supervisor.lib",
        "include/cpm.lib",
        "include/cpmish.lib",
        ".+addresses",
    ],
)

# Builds the memory image. This is a 64kB file containing the entire CP/M
# memory image, including the supervisor at the bottom.

ld80(
    name="memory_img",
    address=0,
    objs={
        0x0000: [
            ".+boot/startup",
        ],
        0x000B: [
            ".+boot/bpb1",
        ],
        0x0038: [
            ".+boot/sirq",
        ],
        0x01FE: [
            ".+boot/bootsig",
        ],
        0x020B: [
            ".+boot/bpb2",
        ],
        0x0400: [".+boot/fat", "arch/nc200/supervisor+variables"],
        0x1000: [".+boot/rootdir", "arch/nc200/supervisor"],
        0x1E00: [
            ".+boot/relauto",
        ],
        cbase: ["third_party/zcpr1"],
        fbase: ["third_party/zsdos"],
        bbase: [".+bios"],
    },
)

# Repackages the memory image as a boot track.

simplerule(
    name="bootfile",
    ins=[".+memory_img"],
    outs=["=bootfile.img"],
    commands=[
        "dd if={ins[0]} of={outs[0]} status=none bs=256 count=36",
        "dd if={ins[0]} of={outs[0]} status=none bs=256 seek=36 skip=231 count=25",
    ],
    label="MKBOOTFILE",
)

unix2cpm(name="readme", src="README.md")

diskimage(
    name="diskimage",
    format="nc200cpm",
    bootfile=".+bootfile",
    map={
        "-readme.txt": ".+readme",
        "asm.com": "cpmtools+asm",
        "asm80.com": "third_party/dr/asm80",
        "bbcbasic.com": "third_party/bbcbasic+bbcbasic_ADM3A",
        "camel80.com": "third_party/camelforth",
        "copy.com": "cpmtools+copy",
        "dump.com": "cpmtools+dump",
        "mkfs.com": "cpmtools+mkfs",
        "qe.com": "cpmtools+qe_NC200",
        "rawdisk.com": "cpmtools+rawdisk",
        "stat.com": "cpmtools+stat",
        "submit.com": "cpmtools+submit",
        "ted.com": "third_party/ted+ted_NC200",
        "z8e.com": "third_party/z8e+z8e_NC200",
    },
)
