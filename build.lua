vars.cpmcflags = {}
vars.cpmldflags = {}

diskimage {
    name = "nc200",
    format = "nc200cpm",
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
    },
}
    