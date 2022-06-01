load("//build:cpm.bzl", "cpm_addresses", "diskimage")
load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")
load("//utils:build.bzl", "objectify", "unix2cpm")

package(default_visibility = ["//visibility:public"])

# Memory layout configuration -----------------------------------------------

# Configure the BIOS size here; this will then emit an addresses.lib file
# which contains the position of the BDOS and CCP.

(cbase, fbase, bbase) = cpm_addresses(
    name = "addresses",
    bios_size = 0x0300
)

# Bootstrap -----------------------------------------------------------------

# The program the NC200 runs on startup.

zmac(
    name = "auto",
    srcs = [
		"auto.z80",
		"//arch/nc200/include:nc200.lib",
    ],
    relocatable = False
)

objectify(
    name = "auto_inc",
    src = ":auto"
)

# FAT boot data -------------------------------------------------------------

[
	zmac(
		name = f.replace(".z80", ""),
		srcs = [
			f,
			":auto_inc",
		]
	)
	for f in glob(["boot/*.z80"])
]

# BIOS ----------------------------------------------------------------------

# The CP/M BIOS itself.

zmac(
    name = "bios",
    srcs = [ 
		"bios.z80",
		"//arch/nc200/include:nc200.lib",
		"//arch/nc200/include:supervisor.lib",
		"//include:cpm.lib",
		"//include:cpmish.lib",
		":addresses",
	]
)

# Builds the memory image. This is a 64kB file containing the entire CP/M
# memory image, including the supervisor at the bottom.

ld80(
    name = "memory_img",
	address = 0,
    objs = {
        0x0000: [ ":boot/startup", ],
        0x000b: [ ":boot/bpb1", ],
        0x0038: [ ":boot/sirq", ],
        0x01fe: [ ":boot/bootsig", ],
        0x020b: [ ":boot/bpb2", ],
        0x0400: [ ":boot/fat", "//arch/nc200/supervisor:variables" ],
        0x1000: [ ":boot/rootdir", "//arch/nc200/supervisor" ],
        0x1e00: [ ":boot/relauto", ],
		cbase:  [ "//third_party/zcpr1" ],
		fbase:  [ "//third_party/zsdos" ],
		bbase:  [ ":bios" ],
    }
)



