from build.ab import simplerule
from third_party.zmac.build import zmac

SRCS = ["as0com", "as1io", "as2scan", "as3sym", "as4sear", "as5oper", "as6main"]

for f in SRCS:
    zmac(name=f, src=f"./{f}.asm", relocatable=False)

simplerule(
    name="asm80",
    ins=[".+" + f for f in SRCS],
    outs=["=asm80.com"],
    commands=["cat {ins[0]} > {outs[0]}"],
    label="CAT",
)
