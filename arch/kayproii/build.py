from build.ab import simplerule
from build.cpm import diskimage
from utils.build import unix2cpm
from third_party.zmac.build import zmac
from third_party.ld80.build import ld80


zmac(name="boot", src="./boot.z80", relocatable=False)

zmac(
    name="bios",
    src="./bios.z80",
    deps=[
        "arch/kayproii/include/kayproii.lib",
        "include/cpm.lib",
        "include/cpmish.lib",
    ],
)

# Builds the memory image.
ld80(
    name="bootfile_mem",
    objs={
        0xE400: ["third_party/zcpr1"],
        0xEC00: ["third_party/zsdos"],
        0xFA00: [".+bios"],
    },
)

# Repackages the memory image as a boot track. This doesn't include the extra
# section of boot image which exists above the directory.
simplerule(
    name="bootfile",
    ins=[".+boot", ".+bootfile_mem"],
    outs=["=bootfile.img"],
    commands=[
        "dd if={ins[0]} of={outs[0]} bs=128 count=1 2> /dev/null",
        "dd if={ins[1]} of={outs[0]} bs=128 seek=1 skip=456 count=16 2> /dev/null",
        "dd if={ins[1]} of={outs[0]} bs=128 seek=17 skip=472 count=23 2> /dev/null",
    ],
    label="MKKAYPRO",
)

unix2cpm(name="readme", src="README.md")

diskimage(
    name="partialimg",
    format="kpii",
    bootfile=".+bootfile",
    map={
        "-readme.txt": ".+readme",
        "dump.com": "cpmtools+dump",
        "stat.com": "cpmtools+stat",
        "asm.com": "cpmtools+asm",
        "copy.com": "cpmtools+copy",
        "submit.com": "cpmtools+submit",
        "bbcbasic.com": "third_party/bbcbasic+bbcbasic_ADM3A",
        "camel80.com": "third_party/camelforth",
        "qe.com": "cpmtools+qe_KAYPROII",
    },
)

# Patches the special extra bit of BDOS/BIOS into the area above the
# directory; yuch.
simplerule(
    name="diskimage",
    ins=[".+partialimg", ".+bootfile_mem"],
    outs=["=diskimage.img"],
    commands=[
        "cp {ins[0]} {outs[0]}",
        "chmod +w {outs[0]}",
        "dd if={ins[1]} of={outs[0]} bs=128 seek=56 skip=495 count=9 conv=notrunc 2> /dev/null",
    ],
    label="MKKAYPRO2",
)
