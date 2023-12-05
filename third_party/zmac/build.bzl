def zmac(name, srcs, relocatable=True, z180=False, dri=False):
    srcfiles = [f for f in srcs if f.endswith(".z80") or f.endswith(".asm") or f.endswith(".mac")]
    if len(srcfiles) != 1:
        fail("you may only have one .z80 or .asm file")
    depfiles = [f for f in srcs if f not in srcfiles]

    ext = ".rel" if relocatable else ".cim"
    relflag = "--rel7" if relocatable else ""
    archflag = "-z" if srcfiles[0].endswith(".z80") else "-8"
    if z180:
        archflag += " --z180"
    if dri:
        archflag += " --dri"
    outfile = name + ext

    hdrflags = ""
    for f in depfiles:
        hdrflags += "-I$$(dirname $(location {})) ".format(f)

    native.genrule(
        name = name,
        srcs = srcfiles + depfiles,
        outs = [outfile],
        tools = ["//third_party/zmac"],
        cmd = "$(execpath //third_party/zmac) --nmnv --zmac -m {} {} {} -o $(execpath {}) $(execpath {})".format(
            relflag, archflag, hdrflags, outfile, srcfiles[0])
    )

# vim: ts=4 sw=4 et

