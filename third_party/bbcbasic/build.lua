include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

local VERSIONS = {
	"ADM3A",
}

local srcs = {"cmos", "eval", "exec", "fpp", "main", "ram", "sorry"}
local generated = {}
for _, f in pairs(srcs) do
    generated[#generated+1] = zmac {
        name = f,
        srcs = { "./"..f..".z80" }
    }
end

for _, version in ipairs(VERSIONS) do
	zmac {
		name = "boot_"..version,
		srcs = { "./"..version:lower().."/boot.z80" }
	}

	ld80 {
		name = "bbcbasic_"..version,
		address = 0x100,
		srcs = {
			"+boot_"..version,
			"-P0200",
			"+main",
			"+exec",
			"+eval",
			"+fpp",
			"+sorry",
			"+cmos",
			"-P3b00",
			"+ram"
		}
	}
end

