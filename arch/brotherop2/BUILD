load("//build:cpm.bzl", "diskimage")
load("//third_party/ld80:build.bzl", "ld80")
load("//third_party/zmac:build.bzl", "zmac")
load("//utils:build.bzl", "unix2cpm")

package(default_visibility = ["//visibility:public"])

genrule(
	name = "font_inc",
	tools = [ "//arch/brotherop2/utils:fontconvert" ],
	srcs = [ "//utils:6x7font.bdf" ],
	outs = [ "font.inc" ],
	cmd = "$(location //arch/brotherop2/utils:fontconvert) $< > $@"
)

zmac(
    name = "boot_o",
    srcs = [
		"boot.z80",
		"//arch/brotherop2/include:brotherop2.lib",
		"//arch/common/utils:tty.lib",
		"//include:cpmish.lib",
		"//include:z180.lib",
		":font_inc",
    ],
)

ld80(
    name = "boot",
	address = 0x5000,
    objs = {
		0x5000: [ ":boot_o" ]
    }
)

[
	zmac(
		name = n,
		srcs = [
			n + ".z80",
			"//arch/brotherop2/include:brotherop2.lib",
			"//arch/common/utils:print.lib",
			"//arch/common/utils:tty.lib",
			"//include:cpm.lib",
			"//include:cpmish.lib",
			"//include:z180.lib",
		]
	)
	for n in [ "bios", "floppy", "tty" ]
]

# This is the bit which CP/M reloads on warm boot (well, some of it).
ld80(
	name = "cpmfile",
	address = 0x9300,
	objs = {
		0x9300: [ "//third_party/zcpr1" ],
		0x9b00: [ "//third_party/zsdos" ],
		0xa900: [
			":bios",
			":floppy",
			":tty",
		]
	}
)

# Produces the FAT bit of the disk image.
zmac(
	name = "fat",
	srcs = [
		"fat.z80",
		":boot",
		":cpmfile"
	]
)

ld80(
	name = "bootfile",
	objs = {
		0x0000: [ ":fat" ]
	}
)

unix2cpm(
    name = "readme",
    src = "README.md"
)

diskimage(
    name = "diskimage",
    format = "brother-op2",
    bootfile = ":bootfile",
    map = {
        "-readme.txt": ":readme",
        "dump.com": "//cpmtools:dump",
        "stat.com": "//cpmtools:stat",
        "asm.com": "//cpmtools:asm",
        "copy.com": "//cpmtools:copy",
        "submit.com": "//cpmtools:submit",
        "rawdisk.com": "//cpmtools:rawdisk",
        "bbcbasic.com": "//third_party/bbcbasic:bbcbasic_ADM3A",
        "camel80.com": "//third_party/camelforth",
        "qe.com": "//cpmtools:qe_BROTHEROP2",
    },
)

