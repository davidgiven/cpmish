from build.c import cprogram

class AckToolchain:
    label = "ACK"
    cfile = ["$(CC) -c -o {outs[0]} {ins[0]} $(CFLAGS) {cflags}"]
    cxxfile = ["$(CXX) -c -o {outs[0]} {ins[0]} $(CFLAGS) {cflags}"]
    clibrary = ["$(AR) cqs {outs[0]} {ins}"]
    cxxlibrary = ["$(AR) cqs {outs[0]} {ins}"]
    cprogram = ["$(CC) -o {outs[0]} {ins} {ldflags} $(LDFLAGS)"]
    cxxprogram = ["$(CXX) -o {outs[0]} {ins} {ldflags} $(LDFLAGS)"]

def ackprogram(**kwargs):
    return cprogram(**kwargs, toolchain=AckToolchain)
