include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"
include "build/z80test.lua"

zmac {
    name = "deblocker",
    srcs = { "./deblocker.z80" },
    deps = {
        "./deblocker.lib",
        "include/cpm.lib"
    },
}

local deblocker_tests = {
    "deblocker_calculate_physical",
    "deblocker_change_block",
    "deblocker_calculate_buffer_address",
    "deblocker_capacity",
}

for _, n in pairs(deblocker_tests) do
    z80test {
        name = n,
        srcs = { "./tests/"..n..".z80" },
        deps = {
            "./deblocker.lib",
            "./deblocker.z80",
            "include/cpm.lib"
        }
    }
end