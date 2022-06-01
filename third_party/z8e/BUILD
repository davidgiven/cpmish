load("//build:cpm.bzl", "binslice", "cpm_addresses", "diskimage")
load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")
load("//utils:build.bzl", "objectify", "unix2cpm")

package(default_visibility = ["//visibility:public"])

VERSIONS = [
	"NC200",
	"WP2450DS",
]

[
	zmac(
		name = "z8e_" + version,
		srcs = [
			"src/z8e.z80",
			"src/terminal.inc",
			version.lower() + "/config.inc",
		] + glob(["src/*.tty"]),
		relocatable = False
	)
	for version in VERSIONS
]

