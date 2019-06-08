normalrule {
    name = "6x7font",
    ins = {
        "./bdftofont.sh",
        "./6x7font.bdf"
    },
    outleaves = { "6x7font.png" },
    commands = {
        "%{ins} %{outs}"
    }
}

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
                "%{ins} > %{outs}"
            }
        }
    end
)
