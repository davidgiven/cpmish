load("//build:ack.bzl", "ack_binary")

libcuss_terminals = [
    "KAYPROII",
    "NC200",
    "BROTHEROP2",
    "BROTHER_WP1",
    "BROTHER_WP2450DS",
    "SPECTRUM_PLUS_THREE",
    "SPECTRUM_NEXT",
    "AGONLIGHTH",
]

def libcuss_ack_binary(**kwargs):
    name = kwargs["name"]
    kwargs.pop("name")

    deps = kwargs.get("deps")
    if not deps:
        deps = []
    else:
        kwargs.pop("deps")

    cflags = kwargs.get("cflags")
    if not cflags:
        cflags = []
    else:
        kwargs.pop("cflags")

    for terminal in libcuss_terminals:
        ack_binary(
            name = name + "_" + terminal,
            deps = deps + ["//cpmtools/libcuss:libcuss_" + terminal],
            cflags = ["-DLIBCUSS_" + terminal] + cflags,
            **kwargs
        )

# vim: ts=4 sw=4 et
    
