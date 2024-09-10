from build.ab import simplerule
from third_party.zmac.build import zmac
from glob import glob

simplerule(
    name="keytab_inc",
    ins=["arch/nc200/utils+mkkeytab"],
    outs=["=keytab.inc"],
    commands=["{ins[0]} > {outs[0]}"],
    label="MKKEYTAB",
)

simplerule(
    name="font_inc",
    ins=["arch/nc200/utils+fontconvert", "utils/6x7font.bdf"],
    outs=["=font.inc"],
    commands=["{ins[0]} {ins[1]} > {outs[0]}"],
    label="FONTCONVERT",
)

zmac(
    name="supervisor",
    src="./supervisor.z80",
    deps=[
        "arch/common/utils/deblocker.lib",
        "arch/nc200/include/nc200.lib",
        "arch/nc200/include/supervisor.lib",
        "arch/nc200+addresses",
        "include/cpm.lib",
        "include/cpmish.lib",
        ".+keytab_inc",
    ]
    + glob("arch/nc200/supervisor/*.inc"),
)

zmac(
    name="variables",
    src="./variables.z80",
    deps=[
        "arch/nc200/include/nc200.lib",
        "arch/nc200/include/supervisor.lib",
        "include/cpm.lib",
        "include/cpmish.lib",
        ".+font_inc",
    ],
)
