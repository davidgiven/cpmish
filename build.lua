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
		["brotherop2.img"] = "arch/brotherop2+diskimage",
		["wp2450ds.img"] = "arch/wp2450ds+diskimage",
    }
}
