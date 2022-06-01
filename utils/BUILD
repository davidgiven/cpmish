package(default_visibility = ["//visibility:public"])

exports_files([ "6x7font.bdf" ])

cc_binary(
	name = "objectify",
	srcs = [ "objectify.c" ]
)

cc_binary(
	name = "objectifyc",
	srcs = [ "objectifyc.c" ]
)

cc_binary(
	name = "unix2cpm",
	srcs = [ "unix2cpm.c" ]
)

cc_library(
	name = "libbdf",
	hdrs = [ "libbdf.h" ],
	srcs = [ "libbdf.c" ],
	strip_include_prefix = ".",
	copts = [
		"-Wno-unused-label"
	]
)

