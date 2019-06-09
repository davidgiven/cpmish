sdccfile {
    name = "cpmcrt",
    srcs = {"./cpmcrt.s"}
}

sdccprogram {
    name = "dump",
    srcs = { "./dump.c" },
}

sdccprogram {
    name = "stat",
    srcs = { "./stat.c" },
}

sdccprogram {
    name = "asm",
    srcs = { "./asm.c" },
}

sdccprogram {
    name = "copy",
    srcs = { "./copy.c" },
}

sdccprogram {
    name = "submit",
    srcs = { "./submit.c" },
}
