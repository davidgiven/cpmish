sdccfile {
    name = "cpmcrt",
    srcs = {"./cpmcrt.s"}
}

sdcclibrary {
    name = "libcpm",
    hdrs = { "./libcpm.h" },
    srcs = { "./*.s" },
}
