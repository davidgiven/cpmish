from third_party.zmac.build import zmac
from third_party.ld80.build import ld80
from glob import glob

VERSIONS = [
    "NC200",
    "WP2450DS",
    "POWERNOTE",
]

for version in VERSIONS:
    zmac(
        name="z8e_" + version,
        src="./src/z8e.z80",
        deps=[
            "./src/terminal.inc",
            "./" + version.lower() + "/config.inc",
        ]
        + glob("third_party/z8e/src/*.tty"),
        relocatable=False,
    )
