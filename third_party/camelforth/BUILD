load("//third_party/zmac:build.bzl", "zmac")

package(default_visibility = ["//visibility:public"])

zmac(
	name = "camelforth",
	srcs = ["camel80.z80"] + glob(["*.inc"]),
	relocatable = False
)

