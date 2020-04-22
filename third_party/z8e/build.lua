include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

local VERSIONS = {
	"NC200",
	"WP2450DS",
}

for _, version in ipairs(VERSIONS) do
	zmac {
		name = "z8e_"..version,
		srcs = { "./src/z8e.z80" },
		deps = {
			"./"..version:lower().."/config.inc",
			"./src/terminal.inc",
			"./src/*.tty",
		},
		relocatable = false
	}
end


