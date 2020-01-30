include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
    name = "boot.o",
    srcs = { "./boot.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib",
		"arch/common/utils/tty.lib"
    },
}

ld80 {
    name = "boot.img",
    srcs = {
		"-P8000",
		"+boot.o"
    }
}

normalrule {
    name = "boot",
    ins = {
        "+boot.img",
    },
    outleaves = { "boot.op2" },
    commands = {
        "dd if=%{ins[1]} of=%{outs} status=none bs=256 skip=128",
    }
}


