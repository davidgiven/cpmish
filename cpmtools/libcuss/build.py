from build.ack import acklibrary, ackprogram
from glob import glob

libcuss_terminals = [
    "KAYPROII",
    "NC200",
    "BROTHEROP2",
    "BROTHER_WP1",
    "BROTHER_WP2450DS",
    "BROTHER_POWERNOTE",
    "SPECTRUM_PLUS_THREE",
    "SPECTRUM_NEXT",
]

for terminal in libcuss_terminals:
    acklibrary(
        name="libcuss_" + terminal,
        cflags=["-DLIBCUSS_" + terminal],
        srcs=glob("cpmtools/libcuss/*.c"),
        hdrs={"libcuss.h": "./libcuss.h"},
    )


def libcuss_ackprogram(name, deps=[], cflags=[], **kwargs):
    for terminal in libcuss_terminals:
        ackprogram(
            name=name + "_" + terminal,
            deps=deps + ["cpmtools/libcuss+libcuss_" + terminal],
            cflags=["-DLIBCUSS_"+terminal] + cflags,
            **kwargs
        )
