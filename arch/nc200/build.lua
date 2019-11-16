include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
    name = "auto",
    srcs = { "./auto.z80" },
    deps = {
        "include/*.lib",
        "./include/*.lib"
    },
    relocatable = false
}

objectify {
    name = "auto_inc",
    srcs = { "+auto" }
}

local bootfiles = filenamesof("./boot/*.z80")
for _, f in pairs(bootfiles) do
    local base = basename(f)
    zmac {
        name = base,
        srcs = { f },
        deps = {
            "include/*.lib",
            "./include/*.lib",
            "+auto_inc"
        }
    }
end

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
        "-P0000", "+startup.z80",
        "-P000b", "+bpb1.z80",
        "-P0038", "+sirq.z80",
        "-P01fe", "+bootsig.z80",
        "-P020b", "+bpb2.z80",
        "-P0400", "+fat.z80",
        "arch/nc200/supervisor+supervisor",
        "-P1000", "+rootdir.z80",
        "-P1e00", "+relauto.z80",
        "-Pe800", "third_party/zcpr1+zcpr",
        "-Pf000", "third_party/zsdos+zsdos",
        "-Pfe00", "+bios",
    }
}

-- Repackages the memory image as a boot track.
normalrule {
    name = "bootfile",
    ins = { "+bootfile_mem" },
    outleaves = { "bootfile.img" },
    commands = {
        "dd if=%{ins[1]} of=%{outs} status=none bs=256 count=36",
        "dd if=%{ins[1]} of=%{outs} status=none bs=256 seek=36 skip=232 count=24"
    }
}

diskimage {
    name = "diskimage",
    format = "nc200cpm",
    bootfile = { "arch/nc200+bootfile" },
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
        ["copy.com"] = "cpmtools+copy",
        ["submit.com"] = "cpmtools+submit",
        ["bbcbasic.com"] = "third_party/bbcbasic+bbcbasic",
        ["qe.com"] = "cpmtools+qe_NC200",
        ["flash.com"] = "arch/nc200/tools+flash",
    },
}
    
