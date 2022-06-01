package(default_visibility = ["//visibility:public"])

cc_binary(
	name = "ld80",
	srcs = glob(["*.c", "*.h"]),
	copts = [
		"-Wno-main",
		"-Wno-stringop-truncation",
		"-Wno-format-overflow",
	]
)

