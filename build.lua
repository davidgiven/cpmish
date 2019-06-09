vars.cflags = { "-g", "-Og" }
vars.ldflags = { "-lm" }
vars.cpmcflags = { "--opt-code-size" }
vars.cpmldflags = {}

diskimage {
    name = "nc200-img",
    format = "nc200cpm",
    bootfile = { "arch/nc200+bootfile" },
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
        ["copy.com"] = "cpmtools+copy",
    },
}
    
installable {
    name = "all",
    map = {
        ["emu"] = "utils/emu+emu",
        ["nc200.img"] = "+nc200-img",
    --    ["osborne1.img"] = "+osborne1-img"
    }
}
