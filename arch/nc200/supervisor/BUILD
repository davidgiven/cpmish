load("//third_party/zmac:build.bzl", "zmac")

package(default_visibility = ["//visibility:public"])

genrule(
    name = "keytab_inc",
	tools = [ "//arch/nc200/utils:mkkeytab" ],
    outs = [ "keytab.inc" ],
	cmd = "$(location //arch/nc200/utils:mkkeytab) > $@"
)

genrule(
	name = "font_inc",
	tools = [ "//arch/nc200/utils:fontconvert" ],
	srcs = [ "//utils:6x7font.bdf" ],
	outs = [ "font.inc" ],
	cmd = "$(location //arch/nc200/utils:fontconvert) $< > $@"
)

zmac(
    name = "supervisor",
    srcs = [
		"supervisor.z80",
		"//arch/common/utils:deblocker.lib",
		"//arch/nc200/include:nc200.lib",
		"//arch/nc200/include:supervisor.lib",
		"//arch/nc200:addresses",
		"//include:cpm.lib",
		"//include:cpmish.lib",
		":keytab_inc",
	] + glob(["*.inc"]),
)

zmac(
    name = "variables",
    srcs = [
		"variables.z80",
		"//arch/nc200/include:nc200.lib",
		"//arch/nc200/include:supervisor.lib",
		"//include:cpm.lib",
		"//include:cpmish.lib",
		":font_inc",
	],
)

