include("build/yacc.lua")

yacc {
    name = "zmacparser",
    srcs = { "./zmac.y" }
}

cprogram {
	name = "doc",
	srcs = { "./doc.c" },
	vars = {
		["+cflags"] = { "-DDOC_DEBUG", "-Ddoc=main" }
	}
}

normalrule {
	name = "doc_inl",
	ins = {
		"+doc",
		"./doc.txt"
	},
	outleaves = { "doc.inl" },
	commands = { "cd %{dirname(ins[2])} && (%{abspath(ins[1])} > /dev/null) && mv doc.inl %{outs}" }
}

cprogram {
    name = "zmac",
    srcs = {
        "./mio.c",
        "./zi80dis.cpp",
		"./doc.c",
        matching(filenamesof("+zmacparser"), "%.c$"),
    },
    deps = {
        "./zi80dis.h",
        "+zmacparser",
		"+doc_inl",
    }
}

definerule("zmac",
    {
        srcs = { type="targets" },
        deps = { type="targets", default={} },
        relocatable = { type="boolean", default=true },
    },
    function (e)
        local firstfilename = filenamesof(e.srcs)[1]
        local _, _, ext = firstfilename:find("%.(%w+)$")

        local archflag = (ext == "z80") and "-z" or "-8"
        local relflag = e.relocatable and "--rel7" or ""
        local ext = e.relocatable and ".rel" or ".cim"

        local hdrpaths = {}
        for _, t in pairs(uniquify(dirname(filenamesof(e.deps)))) do
            hdrpaths[#hdrpaths+1] = "-I"..t
        end

        return normalrule {
            name = e.name,
            ins = {
                "third_party/zmac+zmac",
                e.srcs
            },
            outleaves = { e.name..ext, e.name..".lst" },
            deps = e.deps,
            commands = {
                "%{ins[1]} --zmac -m "..relflag.." "..archflag.." -o %{outs[1]} -o %{outs[2]} %{hdrpaths} %{ins[2]}"
            },
            vars = {
                hdrpaths = hdrpaths,
            }
        }
    end
)
