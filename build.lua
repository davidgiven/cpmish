vars.cflags = { "-g", "-Og" }
vars.ldflags = { "-lm" }
vars.cpmcflags = { "--opt-code-size" }
vars.cpmldflags = {}

installable {
    name = "all",
    map = {
        ["emu"] = "utils/emu+emu",
        ["nc200.img"] = "arch/nc200+diskimage",
        ["kayproii.img"] = "arch/kayproii+diskimage",
    }
}
