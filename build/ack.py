from build.ab import emit
from build.c import cprogram, clibrary

emit(
    """
ACKCC ?= ack
ACKAAL ?= aal
     """
)


class AckToolchain:
    label = "ACK"
    cfile = ["$(ACKCC) -mcpm -c -o {outs[0]} {ins[0]} $(ACKCFLAGS) {cflags}"]
    cxxfile = (["false"],)
    clibrary = ["$(ACKAAL) cq {outs[0]} {ins}"]
    cxxlibrary = (["false"],)
    cprogram = ["$(ACKCC) -mcpm -.c -o {outs[0]} {ins} {ldflags} $(ACKLDFLAGS)"]
    cxxprogram = (["false"],)


def acklibrary(**kwargs):
    return clibrary(**kwargs, toolchain=AckToolchain)


def ackprogram(**kwargs):
    return cprogram(**kwargs, toolchain=AckToolchain)
