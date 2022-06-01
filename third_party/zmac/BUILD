load("//build:build.bzl", "bison")

package(default_visibility = ["//visibility:public"])

bison(
	name = "parser",
	src = "zmac.y"
)

cc_binary(
	name = "doc",
	srcs = ["doc.c"],
	defines = ["DOC_DEBUG", "doc=main"],
	copts = ["-Wno-main"],
)

genrule(
	name = "doc_inl",
	tools = [":doc"],
	outs = ["doc.inl"],
	srcs = ["doc.txt"],
	cmd = "EXE=$$PWD/$(execpath :doc) && cp $(execpath doc.txt) $(RULEDIR) && (cd $(@D) && $$EXE) >/dev/null && rm $(RULEDIR)/doc.txt",
)

cc_binary(
	name = "zmac",
	copts = [
		"-Wno-unused-result"
	],
	srcs = [
		":doc.inl",
		":parser.c",
		":parser.h",
		"doc.c",
		"mio.c",
		"mio.h",
		"zi80dis.cpp",
		"zi80dis.h",
	],
	includes = [
		":doc.inl/.."
	],
)
