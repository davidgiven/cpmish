include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

local PLATFORMS = {
	"NC200"
}

-- These files are the same for all platforms.

for _, f in ipairs({ "cpmio", "main", "fileio", "cmds" }) do
    zmac {
        name = f,
        srcs = { "./"..f..".mac" }
    }
end

local srcs = {
   "cpmio",
   "main",
   "termdef",
   "fileio", 
   "scrn",
   "cmds"
}

for _, platform in ipairs(PLATFORMS) do
	-- These need to be compiled individually per platform.
	--
	for _, f in ipairs({ "termdef", "scrn" }) do
		zmac {
			name = f.."_"..platform,
			srcs = { "./"..f..".mac" },
			deps = { "./"..platform:lower().."/config.inc" }
		}
	end

	ld80 {
	   name = "ted_"..platform,
	   address = 0x100,
	   srcs = {
		   "+termdef_"..platform,
		   "+main",
		   "+fileio", 
		   "+scrn_"..platform,
		   "+cmds",
		   "+cpmio"
		}
	}
end

