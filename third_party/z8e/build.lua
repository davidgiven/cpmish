include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

zmac {
	name = "z8e_o",
	srcs = { "./z8e.z80" }
}

ld80 {
	name = "z8e",
	address = 0x100,
	srcs = { "+z8e_o" }
}

