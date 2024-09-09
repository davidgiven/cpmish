from third_party.zmac.build import zmac
from third_party.ld80.build import ld80

VERSIONS = [
    "NC200",
    "WP2450DS",
    "POWERNOTE",
]

for f in [
    "cpmio",
    "main",
    "fileio",
    "cmds",
]:
    zmac(name=f, src=f"./{f}.mac", deps=["./ted.inc"])

for v in VERSIONS:
    zmac(
        name="termdef_" + v,
        src="./termdef.mac",
        deps=[
            "./" + v.lower() + "/config.inc",
            "./ted.inc",
        ],
    )
    zmac(
        name="scrn_" + v,
        src="./scrn.mac",
        deps=[
            "./" + v.lower() + "/config.inc",
            "./ted.inc",
        ],
    )
    ld80(
        name="ted_" + v,
        address=0x0100,
        objs={
            0x0100: [
                ".+main",
                ".+termdef_" + v,
                ".+fileio",
                ".+scrn_" + v,
                ".+cmds",
                ".+cpmio",
            ]
        },
    )
