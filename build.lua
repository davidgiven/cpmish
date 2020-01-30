vars.cflags = { "-g", "-Og" }
vars.ldflags = { "-lm" }
vars.cpmcflags = { "-O6" }
vars.cpmldflags = {}

installable {
    name = "all",
    map = {
        ["emu"] = "utils/emu+emu",
        ["nc200.img"] = "arch/nc200+diskimage",
        ["kayproii.img"] = "arch/kayproii+diskimage",
		["cpmish.op2"] = "arch/brotherop2+boot",
    }
}
