from build.ab import simplerule
from build.cpm import cpm_addresses, diskimage
from utils.build import unix2cpm
from third_party.zmac.build import zmac
from third_party.ld80.build import ld80

(cbase, fbase, bbase) = cpm_addresses(name="addresses", bios_size=0x0a00)

zmac(name="boot", src="./boot.z80", relocatable=False)

zmac(
    name="bios",
    src="./bios.z80",
    deps=[
        "arch/nano-z80/include/nano-z80.lib",
        "include/cpm.lib",
        "include/cpmish.lib",
        ".+addresses",
    ],
)

zmac(
    name="tty",
    src="./tty.z80",
    deps=[
        "arch/nano-z80/include/nano-z80.lib",
        "include/cpm.lib",
        "include/cpmish.lib",
        "arch/common/utils/tty.lib",
        "arch/common/utils/print.lib",
    ],
)


# Builds the memory image.
ld80(
    name="bootfile_mem",
    objs={
        cbase: ["third_party/zcpr1"],
        fbase: ["third_party/zsdos"],
        bbase: [".+bios",".+tty",],
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
        "dd if={ins[1]} of={outs[0]} bs=128 seek=4 skip=446 count=64 2> /dev/null",
    ],
    label="nanoZ80",
)

unix2cpm(name="readme", src="README.md")

diskimage(
    name="diskimage",
    format="nanoz80",
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
        "qe.com": "cpmtools+qe_NANOZ80",
        "rawdisk.com": "cpmtools+rawdisk",
        "mkfs.com": "cpmtools+mkfs",
        "baudrate.com": "arch/nano-z80/tools+baudrate",
        "nanoterm.com": "arch/nano-z80/tools+nanoterm",
        "colorfg.com": "arch/nano-z80/tools+colorfg",
        "colorbg.com": "arch/nano-z80/tools+colorbg",
        "cls.com" : "arch/nano-z80/tools+cls",
        "arrowkey.com" : "arch/nano-z80/tools+arrowkey",
        "imgview.com" : "arch/nano-z80/tools+imgview",
        },
)

