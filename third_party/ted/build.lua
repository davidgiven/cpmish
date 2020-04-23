include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

local srcs = {
   "cpmio",
   "main",
   "termdef",
   "fileio", 
   "scrn",
   "cmds"
}

for _, f in pairs(srcs) do
    zmac {
        name = f,
        srcs = { "./"..f..".mac" }
    }
end

ld80 {
   name = "ted",
   address = 0x100,
   srcs = {
	   "+termdef",
	   "+main",
	   "+fileio", 
	   "+scrn",
	   "+cmds",
	   "+cpmio"
	}
}

