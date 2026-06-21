from third_party.zmac.build import zmac
from build.ack import ackprogram

zmac(
    name="baudrate",
    relocatable=False,
    src="./baudrate.z80",
    deps=[
        "arch/nano-z80+addresses",
    ],
)

zmac(
    name="colorfg",
    relocatable=False,
    src="./colorfg.z80",
    deps=[
        "arch/nano-z80+addresses",
    ],
)

zmac(
    name="colorbg",
    relocatable=False,
    src="./colorbg.z80",
    deps=[
        "arch/nano-z80+addresses",
    ],
)

zmac(
    name="cls",
    relocatable=False,
    src="./cls.z80",
    deps=[
        "arch/nano-z80+addresses",
    ],
)

zmac(
    name="arrowkey",
    relocatable=False,
    src="./arrowkey.z80",
    deps=[
        "arch/nano-z80+addresses",
    ],
)

ackprogram(
    name="nanoterm",
    srcs=["./nanoterm.c", "./uart.s"],
)

ackprogram(
    name="imgview",
    srcs=["./imgview.c", "./gfx.s"],
)
