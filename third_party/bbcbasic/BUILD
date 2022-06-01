load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")

package(default_visibility = ["//visibility:public"])

VERSIONS = [
	"ADM3A"
]

[
	zmac(
		name = f,
		srcs = [ f+".z80" ]
	)
	for f in [
		"cmos",
		"eval",
		"exec",
		"fpp",
		"main",
		"ram",
		"sorry"
	]
]

[
	[
		zmac(
			name = "boot_" + v,
			srcs = [ v.lower() + "/boot.z80" ]
		),
		ld80(
			name = "bbcbasic_" + v,
			objs = {
				0x0100: [ ":boot_" + v ],
				0x0200: [ ":main", ":exec", ":eval", ":fpp", ":sorry", ":cmos" ],
				0x3b00: [ ":ram" ]
			}
		)
	]
	for v in VERSIONS
]

