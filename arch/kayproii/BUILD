load("//build:cpm.bzl", "diskimage")
load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")
load("//utils:build.bzl", "unix2cpm")

package(default_visibility = ["//visibility:public"])

zmac(
    name = "boot",
    srcs = [ "boot.z80" ],
    relocatable = False
)

zmac(
    name = "bios",
    srcs = [
		"bios.z80",
		"//arch/kayproii/include:kayproii.lib",
		"//include:cpm.lib",
		"//include:cpmish.lib",
    ],
)

# Builds the memory image.
ld80(
    name = "bootfile_mem",
    objs = {
        0xe400: [ "//third_party/zcpr1" ],
        0xec00: [ "//third_party/zsdos" ],
        0xfa00: [ ":bios" ],
    }
)

# Repackages the memory image as a boot track. This doesn't include the extra
# section of boot image which exists above the directory.
genrule(
    name = "bootfile",
    srcs = [
        ":boot",
        ":bootfile_mem"
    ],
    outs = [ "bootfile.img" ],
    cmd = " && ".join([
        "dd if=$(location :boot) of=$@ bs=128 count=1 2> /dev/null",
        "dd if=$(location :bootfile_mem) of=$@ bs=128 seek=1 skip=456 count=16 2> /dev/null",
        "dd if=$(location :bootfile_mem) of=$@ bs=128 seek=17 skip=472 count=23 2> /dev/null",
    ])
)

unix2cpm(
    name = "readme",
    src = "README.md"
)

diskimage(
    name = "partialimg",
    format = "kpii",
    bootfile = ":bootfile",
    map = {
        "-readme.txt": ":readme",
        "dump.com": "//cpmtools:dump",
        "stat.com": "//cpmtools:stat",
        "asm.com": "//cpmtools:asm",
        "copy.com": "//cpmtools:copy",
        "submit.com": "//cpmtools:submit",
        "bbcbasic.com": "//third_party/bbcbasic:bbcbasic_ADM3A",
        "camel80.com": "//third_party/camelforth",
        "qe.com": "//cpmtools:qe_KAYPROII",
    },
)

# Patches the special extra bit of BDOS/BIOS into the area above the
# directory; yuch.
genrule(
    name = "diskimage",
	srcs = [
		":partialimg",
		":bootfile_mem"
	],
    outs = [ "diskimage.img" ],
	cmd = " && ".join([
        "cp $(location :partialimg) $@",
		"chmod +w $@",
        "dd if=$(location :bootfile_mem) of=$@ bs=128 seek=56 skip=495 count=9 conv=notrunc 2> /dev/null"
    ])
)

