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
    deps = {
        "include/*.lib",
        "./include/*.lib"
    },
}

-- Builds the memory image.
ld80 {
    name = "bootfile_mem",
    srcs = {
        "-Pe400", "third_party/zcpr1+zcpr",
        "-Pec00", "third_party/zsdos+zsdos",
        "-Pfa00", "+bios",
    }
}

-- Repackages the memory image as a boot track. This doesn't include the extra
-- section of boot image which exists above the directory.
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
    }
}

diskimage {
    name = "partialimg",
    format = "kpii",
    bootfile = { "arch/kayproii+bootfile" },
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
        ["copy.com"] = "cpmtools+copy",
        ["submit.com"] = "cpmtools+submit",
    },
}
    
-- Patches the special extra bit of BDOS/BIOS into the area above the
-- directory; yuch.
normalrule {
    name = "diskimage",
    ins = {
        "+partialimg",
        "+bootfile_mem"
    },
    outleaves = { "diskimage.img" },
    commands = {
        "cp %{ins[1]} %{outs}",
        "truncate -s 204800 %{outs}",
        "dd if=%{ins[2]} of=%{outs} status=none bs=128 seek=56 skip=495 count=9 conv=notrunc"
    }
}
