include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
	name = "boot_img",
	srcs = { "./boot.z80" },
	deps = {
        "include/*.lib",
        "./include/*.lib",
	},
	relocatable = false
}

normalrule {
    name = "diskimage",
    ins = { "+boot_img" },
    outleaves = { "wp1.img" },
    commands = {
        "brother120tool --create %{outs} '*boot=%{ins[1]}'"
    }
}


