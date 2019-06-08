include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "utils/build.lua"

zmac {
    name = "auto",
    srcs = { "./auto.z80" },
    deps = { "./include/*.lib" },
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
            "./include/*.lib",
            "+auto_inc"
        }
    }
end

zmac {
    name = "bios",
    srcs = { "./bios.z80" },
    deps = { "./include/*.lib" },
}

ld80 {
    name = "bootfile",
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
