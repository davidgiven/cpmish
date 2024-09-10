from third_party.zmac.build import zmac
from third_party.ld80.build import ld80

VERSIONS = ["ADM3A"]

for f in ["cmos", "eval", "exec", "fpp", "main", "ram", "sorry"]:
    zmac(name=f, src=f"./{f}.z80")

for v in VERSIONS:
    zmac(name=f"boot_{v}", src=f"./{v.lower()}/boot.z80")
    ld80(
        name=f"bbcbasic_{v}",
        objs={
            0x0100: [".+boot_" + v],
            0x0200: [
                ".+main",
                ".+exec",
                ".+eval",
                ".+fpp",
                ".+sorry",
                ".+cmos",
            ],
            0x3B00: [".+ram"],
        },
    )
