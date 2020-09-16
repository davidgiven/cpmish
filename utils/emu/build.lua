include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
    name = "biosbdos_rel",
    srcs = { "./biosbdos.z80" }
}

ld80 {
    name = "biosbdos_memimg",
    srcs = {
        "-Pf700", "third_party/zcpr1+zcpr",
        "-Pff00", "+biosbdos_rel"
    }
}
 
binslice {
	name = "biosbdos_cim",
	src = "+biosbdos_memimg",
	start = 0xf700
}

objectifyc {
    name = "biosbdos_cim_h",
    srcs = { "+biosbdos_cim" }
}

cprogram {
    name = "emu",
    srcs = { "./*.c" },
    deps = {
		"+biosbdos_cim_h",
		"third_party/libz80ex+libz80ex"
	},
    vars = {
        ["+ldflags"] = { "-lreadline" } 
    }
}
