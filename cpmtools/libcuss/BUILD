load("//build:ack.bzl", "ack_library")
load("libcuss.bzl", "libcuss_terminals")

package(default_visibility = ["//visibility:public"])

[
	ack_library(
		name = "libcuss_" + terminal,
		cflags = ["-DLIBCUSS_" + terminal],
		srcs = glob(["*.c"]),
		hdrs = ["libcuss.h"],
	)
	for terminal in libcuss_terminals
]

# vim: ts=4 sw=4 et

