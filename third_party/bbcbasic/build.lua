include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

local srcs = {"cmos", "eval", "exec", "fpp", "main", "patch", "ram", "sorry"}
local generated = {}
for _, f in pairs(srcs) do
    generated[#generated+1] = zmac {
        name = f,
        srcs = { "./"..f..".z80" }
    }
end

ld80 {
    name = "bbcbasic_img",
    srcs = {
        "+patch",
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

bintocom {
    name = "bbcbasic",
    srcs = { "+bbcbasic_img" }
}
