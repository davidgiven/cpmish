load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")

package(default_visibility = ["//visibility:public"])

SRCS = [
	"as0com",
	"as1io",
	"as2scan",
	"as3sym",
	"as4sear",
	"as5oper",
	"as6main"
]

[
	zmac(
		name = f,
		srcs = [ f + ".asm" ],
		relocatable = False,
	)
	for f in SRCS
]

genrule(
	name = "asm80",
	srcs = [ ":"+f for f in SRCS ],
	outs = [ "asm80.com" ],
	cmd = "cat $(SRCS) > $@"
)

