load("//build:ack.bzl", "ack_binary")
load("//cpmtools/libcuss:libcuss.bzl", "libcuss_ack_binary")

package(default_visibility = ["//visibility:public"])

SIMPLE_TOOLS = [
	"asm",
	"copy",
	"dump",
	"mkfs",
	"rawdisk",
	"stat",
	"submit"
]

LIBCUSS_TOOLS = [
	"qe",
]

[
	ack_binary(
		name = tool,
		srcs = [tool + ".c"]
	)
	for tool in SIMPLE_TOOLS
]

[
	libcuss_ack_binary(
		name = tool,
		srcs = [tool + ".c"]
	)
	for tool in LIBCUSS_TOOLS
]

