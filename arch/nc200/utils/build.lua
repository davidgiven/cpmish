cprogram {
    name = "mkkeytab",
    srcs = { "./mkkeytab.c" }
}

cprogram {
    name = "fontconvert",
    srcs = { "./fontconvert.c" },
    deps = { "third_party/libstb+libstb" },
}
