include "cpmtools/libcuss/build.lua"

ackprogram {
    name = "dump",
    srcs = { "./dump.c" },
}

ackprogram {
    name = "stat",
    srcs = { "./stat.c" },
}

ackprogram {
    name = "asm",
    srcs = { "./asm.c" },
}

ackprogram {
    name = "copy",
    srcs = { "./copy.c" },
}

ackprogram {
    name = "submit",
    srcs = { "./submit.c" },
}

ackprogram {
    name = "mkfs",
    srcs = { "./mkfs.c" },
}

ackprogram {
    name = "rawdisk",
    srcs = { "./rawdisk.c" },
}

libcussprogram {
    name = "qe",
    srcs = { "./qe.c" },
}
