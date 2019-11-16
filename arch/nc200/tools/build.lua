include "cpmtools/libcuss/build.lua"

ackprogram {
    name = "flash",
    srcs = {
        "./flash.c",
        "./pcmcia-tools.s"
    },
}
