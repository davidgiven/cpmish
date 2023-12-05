load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")

package(default_visibility = ["//visibility:public"])

VERSIONS = [
	"NC200",
	"WP2450DS",
	"POWERNOTE",
]

[
	zmac(
		name = f,
		srcs = [
			f + ".mac",
			"ted.inc",
		]
	)
	for f in [
		"cpmio",
		"main",
		"fileio",
		"cmds",
	]
]

[
	[
		zmac(
			name = "termdef_" + v,
			srcs = [
				"termdef.mac",
				v.lower() + "/config.inc",
				"ted.inc",
			]
		),
		zmac(
			name = "scrn_" + v,
			srcs = [
				"scrn.mac",
				v.lower() + "/config.inc",
				"ted.inc",
			]
		),
		ld80(
			name = "ted_" + v,
			address = 0x0100,
			objs = {
				0x0100: [
					":main",
					":termdef_" + v,
					":fileio",
					":scrn_" + v,
					":cmds",
					":cpmio"
				]
			}
		)
	]
	for v in VERSIONS
]

