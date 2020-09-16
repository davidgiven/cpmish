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
				"tail -c+"..(e.address+1).." %{outs[1]}.all > %{outs[1]}"
            }
        }
    end
)

definerule("bintocom",
    {
        srcs = { type="table" },
    },
    function (e)
		return binrule {
			name = e.name,
			src = { e.ins[1] },
			start = 0x100
		}
    end
)
