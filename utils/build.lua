cprogram {
    name = "objectify",
    srcs = { "./objectify.c" }
}

definerule("objectify",
    {
        srcs = { type="targets" },
    },
    function (e)
        return normalrule {
            name = e.name,
            ins = {
                "utils+objectify",
                e.srcs,
            },
            outleaves = { e.name..".inc" },
            commands = {
                "%{ins[1]} < %{ins[2]} > %{outs}"
            }
        }
    end
)

cprogram {
    name = "objectifyc",
    srcs = { "./objectifyc.c" }
}

definerule("objectifyc",
    {
        srcs = { type="targets" },
    },
    function (e)
        return normalrule {
            name = e.name,
            ins = {
                "utils+objectifyc",
                e.srcs,
            },
            outleaves = { e.name..".h" },
            commands = {
                "%{ins[1]} < %{ins[2]} > %{outs}"
            }
        }
    end
)

cprogram {
    name = "unix2cpm",
    srcs = { "./unix2cpm.c" }
}

definerule("unix2cpm",
    {
        srcs = { type="targets" },
    },
    function (e)
        return normalrule {
            name = e.name,
            ins = {
                "utils+unix2cpm",
                e.srcs,
            },
            outleaves = { e.name..".txt" },
            commands = {
                "%{ins[1]} < %{ins[2]} > %{outs}"
            }
        }
    end
)

clibrary {
	name = "libbdf",
	srcs = { "./libbdf.c" },
	hdrs = { "./libbdf.h" }
}

