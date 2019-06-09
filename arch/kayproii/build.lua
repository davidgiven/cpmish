include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
    name = "boot",
    srcs = { "./boot.z80" },
    relocatable = false
}

zmac {
    name = "bios",
    srcs = { "./bios.z80" },
    deps = { "./include/*.lib" },
}

-- Builds the memory image.
ld80 {
    name = "bootfile_mem",
    srcs = {
        "-Pe400", "third_party/zcpr1+zcpr",
        "-Pec00", "third_party/zsdos+zsdos",
        "-Pf900", "+bios",
    }
}

-- Repackages the memory image as a boot track.
normalrule {
    name = "bootfile",
    ins = {
        "+boot",
        "+bootfile_mem"
    },
    outleaves = { "bootfile.img" },
    commands = {
        "dd if=%{ins[1]} of=%{outs} status=none bs=128 count=1",
        "dd if=%{ins[3]} of=%{outs} status=none bs=128 seek=1 skip=456 count=16",
        "dd if=%{ins[3]} of=%{outs} status=none bs=128 seek=17 skip=472 count=23",
        "dd if=%{ins[3]} of=%{outs} status=none bs=128 seek=56 skip=495 count=9"
    }
}
