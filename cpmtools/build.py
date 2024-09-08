from build.ack import ackprogram
from cpmtools.libcuss.build import libcuss_ackprogram

SIMPLE_TOOLS = ["asm", "copy", "dump", "mkfs", "rawdisk", "stat", "submit"]

LIBCUSS_TOOLS = [
    "qe",
]

for tool in SIMPLE_TOOLS:
    ackprogram(name=tool, srcs=[f"./{tool}.c"])

for tool in LIBCUSS_TOOLS:
    libcuss_ackprogram(name=tool, srcs=[f"./{tool}.c"])
