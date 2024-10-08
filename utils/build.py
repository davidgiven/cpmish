from build.ab import simplerule, Rule, Target
from build.c import cprogram, clibrary

cprogram(name="objectify", srcs=["./objectify.c"])
cprogram(name="objectifyc", srcs=["./objectifyc.c"])
cprogram(name="unix2cpm", srcs=["./unix2cpm.c"])

clibrary(
    name="libbdf",
    srcs=["./libbdf.c"],
    hdrs={"libbdf.h": "./libbdf.h"},
    cflags=["-Wno-unused-label"],
)


@Rule
def unix2cpm(self, name, src: Target):
    simplerule(
        replaces=self,
        ins=["utils+unix2cpm", src],
        outs=[f"={self.localname}.txt"],
        commands=["{ins[0]} < {ins[1]} > {outs[0]}"],
        label="UNIX2CPM",
    )

@Rule
def objectify(self, name, src:Target):
    simplerule(
        replaces=self,
        ins = [ src, "utils+objectify" ],
        outs = [ f"={self.localname}.inc" ],
        commands=["{ins[1]} < {ins[0]} > {outs[0]}"],
        label="OBJECTIFY")
