from build.ab import Rule, filenameof, simplerule
from build.c import cprogram
from glob import glob

cprogram(
    name="ld80",
    srcs=glob("third_party/ld80/*.[ch]"),
    cflags=[
        "-Wno-main",
        "-Wno-stringop-truncation",
        "-Wno-format-overflow",
    ],
)


class ListOfTargetsMap:
    def convert(value, target):
        if not value:
            return {}
        return {k: [target.targetof(v) for v in vs] for k, vs in value.items()}


@Rule
def ld80(self, name, address=0x0100, objs: ListOfTargetsMap = {}):
    args = []
    targets = []
    for addr in objs:
        args += [f"-P{hex(addr)}"]
        for obj in objs[addr]:
            args += [filenameof(obj)]
            targets += [obj]

    simplerule(
        replaces=self,
        ins=["third_party/ld80"] + targets,
        outs=[f"={self.localname}.cim"],
        commands=[
            "{ins[0]} -m -O bin -o {outs[0]}.all -s /dev/null "
            + (" ".join(args)),
            f"tail -c+{address+1} {{outs[0]}}.all > {{outs[0]}}",
        ],
        label="LD80",
    )


# vim: ts=4 sw=4 et
