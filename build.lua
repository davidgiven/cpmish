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
        ["submit.com"] = "cpmtools+submit",
    },
}
    
diskimage {
    name = "kayproii-img",
    format = "kpii",
    bootfile = { "arch/kayproii+bootfile" },
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
        ["copy.com"] = "cpmtools+copy",
        ["submit.com"] = "cpmtools+submit",
    },
}
    
installable {
    name = "all",
    map = {
        ["emu"] = "utils/emu+emu",
        ["nc200.img"] = "+nc200-img",
        ["kayproii.img"] = "+kayproii-img"
    }
}
