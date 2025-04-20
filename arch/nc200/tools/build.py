from third_party.zmac.build import zmac
from build.ack import ackprogram

zmac(
    name="flipdisk",
    relocatable=False,
    src="./flipdisk.z80",
    deps=[
        "arch/nc200+addresses",
    ],
)

ackprogram(
    name="flash",
    srcs=["./flash.c", "./pcmcia-tools.s"],
)
