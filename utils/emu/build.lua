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

normalrule {
    name = "biosbdos_cim",
    ins = { "+biosbdos_memimg" },
    outleaves = { "biosbdos.cim" },
    commands = {
        "dd if=%{ins[1]} of=%{outs} status=none bs=1 skip=63232"
    }
}

objectifyc {
    name = "biosbdos_cim_h",
    srcs = { "+biosbdos_cim" }
}

cprogram {
    name = "emu",
    srcs = { "./*.c" },
    deps = { "+biosbdos_cim_h" },
    vars = {
        ["+ldflags"] = { "-lz80ex", "-lz80ex_dasm", "-lreadline" } 
    }
}
