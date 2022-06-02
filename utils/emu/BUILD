load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")
load("//utils:build.bzl", "objectifyc")

package(default_visibility = ["//visibility:public"])

zmac(
	name = "biosbdos_rel",
	srcs = [
		"biosbdos.z80"
	],
	relocatable = True
)

ld80(
	name = "biosbdos_cim",
	address = 0xf700,
	objs = {
		0xf700: [ "//third_party/zcpr1" ],
		0xff00: [ ":biosbdos_rel" ]
	}
)

objectifyc(
	name = "biosbdos_cim_h",
	src = ":biosbdos_cim"
)

cc_library(
	name = "biosbdos_cim_lib",
	hdrs = [ ":biosbdos_cim_h" ],
	strip_include_prefix = ".",
)

cc_binary(
	name = "emu",
	copts = [
		"-I/usr/include/z80ex",
		"-Wno-unused-result",
	],
	linkopts = [
		"-lz80ex",
		"-lz80ex_dasm",
		"-lreadline",
	],
	srcs = glob(["*.c"]) + [
		"globals.h",
	],
	deps = [
		":biosbdos_cim_lib"
	]
)

