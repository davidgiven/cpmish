clibrary {
    name = "libz80ex",
	srcs = {
		"./z80ex.c",
		"./z80ex_dasm.c",
	},
	deps = {
		"./include/z80ex.h",
		"./include/z80ex_dasm.h",
	},
    hdrs = { "./include/*.h" },
	vars = {
		["+cflags"] = {
			"-DENDIANNESS=WORDS_LITTLE_ENDIAN",
			"-DZ80EX_API_REVISION=1",
			"-DZ80EX_VERSION_MAJOR=1",
			"-DZ80EX_VERSION_MINOR=21",
			"-DZ80EX_RELEASE_TYPE=cpmish",
			"-DZ80EX_VERSION_STR=1.1.21-cpmish",
			"-Ithird_party/libz80ex/include",
		},
	}
}

