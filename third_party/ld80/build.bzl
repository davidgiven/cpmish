def ld80(name, address=0x0100, objs={}):
    args = []
    targets = []
    for addr in objs:
        args += ["-P$$(printf '%%x' %d)" % addr]
        for obj in objs[addr]:
            args += ["$(location "+obj+")"]
            targets += [obj]

    out_file = name + ".cim"
    native.genrule(
        name = name,
        srcs = targets,
        outs = [out_file],
        tools = ["//third_party/ld80"],
        cmd = " && ".join([
            "$(location //third_party/ld80) -m -O bin -o $(location "+out_file+").all -s /dev/null "+
                (" ".join(args)),
            "tail -c+{address} $(location {out_file}).all > $(location {out_file})".format(
                address=address+1, out_file=out_file),
            "rm $(location {}).all".format(out_file)
        ])
    )

# vim: ts=4 sw=4 et

