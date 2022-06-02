load("//build:cpm.bzl", "z80test")

TESTS = [
	"deblocker_calculate_buffer_address",
	"deblocker_calculate_physical",
	"deblocker_capacity",
	"deblocker_change_block",
]

[
	z80test(
		name = test,
		srcs = [
			test + ".z80",
			"//arch/common/utils:deblocker.lib",
		]
	)
	for test in TESTS
]

