from build.ab import simplerule, Rule, Target, Targets, filenamesof, filenameof
from build.yacc import bison
from build.c import cprogram, cxxprogram, clibrary
from os.path import *

bison(name="parser", src="./zmac.y")

cprogram(
    name="doc",
    srcs=["./doc.c"],
    cflags=["-DDOC_DEBUG", "-Ddoc=main", "-Wno-main"],
)

simplerule(
    name="doc_inl",
    outs=["=doc.inl"],
    ins=["./doc.txt"],
    deps=[".+doc"],
    commands=[
        "cp {ins[0]} {dir}",
        "(cd {dir} && $(abspath {deps[0]}) > /dev/null)",
    ],
    label="DOC_INL",
)

clibrary(name="doc_inl_lib", hdrs={"doc.inl": ".+doc_inl"})

clibrary(
    name="zmaclib",
    srcs=[
        ".+parser",
        "./doc.c",
        "./mio.c",
        "./mio.h",
    ],
    deps=[
        ".+doc_inl_lib",
    ],
)

cxxprogram(
    name="zmac",
    cflags=["-Wno-unused-result"],
    srcs=[
        "./zi80dis.cpp",
        "./zi80dis.h",
    ],
    deps=[".+zmaclib"],
)


@Rule
def zmac(
    self,
    name,
    src: Target = None,
    deps: Targets = [],
    relocatable=True,
    z180=False,
    dri=False,
):
    ext = ".rel" if relocatable else ".cim"
    relflag = "--rel7" if relocatable else ""
    archflag = "-z" if filenameof(src).endswith(".z80") else "-8"
    if z180:
        archflag += " --z180"
    if dri:
        archflag += " --dri"
    outfile = f"={self.localname}{ext}"

    hdrflags = " ".join([f"-I{dirname(f)}" for f in filenamesof(deps)])

    simplerule(
        replaces=self,
        ins=[src, "third_party/zmac"] + deps,
        outs=[f"={self.localname}{ext}"],
        commands=[
            f"{{ins[1]}} --nmnv --zmac -m {relflag} {archflag} {hdrflags} -o {{outs[0]}} {{ins[0]}}"
        ],
        label="ZMAC",
    )


# vim: ts=4 sw=4 et
