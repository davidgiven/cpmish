include "third_party/ld80/build.lua"
include "third_party/zmac/build.lua"

definerule("z80test",
	{
		srcs = { type="targets" },
		deps = { type="targets", default={} },
    },
    function (e)
        local rel = zmac {
            name = e.name.."_zmac",
            srcs = e.srcs,
            deps = e.deps,
        }

        local cim = ld80 {
            name = e.name.."_cim",
            srcs = { "-P0x0100", rel }
        }

        local com = bintocom {
            name = e.name.."_com",
            srcs = { cim }
        }

        normalrule {
            name = e.name.."_test",
            ins = {
                "utils/emu+emu",
                com
            },
            outleaves = { "log" },
            commands = {
                "%{ins[1]} %{ins[2]} > %{outs} && test ! -s %{outs}"
            }
        }
    end
)


