cprogram {
    name = "ld80",
    srcs = {
        "./main.c",
        "./readobj.c",
        "./section.c",
        "./symbol.c",
        "./fixup.c",
        "./do_out.c",
        "./optget.c"
    }
}

definerule("ld80",
    {
        srcs = { type="table" },
		address = { type="number", default=0 },
    },
    function (e)
        local args = {}
        local deps = {}

        for _, src in ipairs(e.srcs) do
            if (type(src) == "string") and src:find("^%-") then
                args[#args+1] = src
            else
                deps[#deps+1] = src
                args[#args+1] = filenamesof(src)[1]
            end
        end

        return normalrule {
            name = e.name,
            outleaves = {
                e.name..".cim",
            },
            ins = {
                "third_party/ld80+ld80",
                deps,
            },
            commands = {
                "%{ins[1]} -m -O bin -o %{outs[1]}.all -s %{outs[1]}.sym "..table.concat(args, " "),
                "dd if=%{outs[1]}.all of=%{outs[1]} status=none bs=1 skip="..e.address
            }
        }
    end
)

definerule("bintocom",
    {
        srcs = { type="table" },
    },
    function (e)
        return normalrule {
            name = e.name,
            outleaves = {
                e.name..".com",
            },
            ins = e.srcs,
            commands = {
                "dd if=%{ins[1]} of=%{outs[1]} status=none bs=128 skip=2"
            }
        }
    end
)
