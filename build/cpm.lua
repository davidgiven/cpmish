definerule("sdccfile",
	{
		srcs = { type="targets" },
		deps = { type="targets", default={} },
		suffix = { type="string", default=".rel" },
	},
	function (e)
		if (#e.srcs ~= 1) then
			error("you can only have one input to a sdccfile")
		end

		local firstfilename = filenamesof(e.srcs)[1]
		local _, _, ext = firstfilename:find("%.(%w+)$")

		if (ext == "c") then
			return cfile {
				name = e.name,
				srcs = e.srcs,
				deps = e.deps,
				suffix = e.suffix,
				commands = {
					"sdcc -mz80 -c -o %{outs} %{ins} %{hdrpaths} %{cpmcflags}"
				}
			}
		elseif (ext == "s") then
			return normalrule {
				name = e.name,
				ins = e.srcs,
				outleaves = { e.name..e.suffix },
				deps = e.deps,
				commands = {
					"sdasz80 -g -o %{outs} %{ins}"
				}
			}
		else
			error("unknown cpmcfile extension '"..ext.."'")
		end
	end
)

definerule("sdcclibrary",
	{
		srcs = { type="targets", default={} },
		hdrs = { type="targets", default={} },
		deps = { type="targets", default={} },
	},
	function (e)
		return clibrary {
			name = e.name,
			srcs = e.srcs,
			hdrs = e.hdrs,
			deps = e.deps,
			suffix = ".rel",
			_cfile = sdccfile,
			commands = {
				"rm -f %{outs[1]}",
				"sdcclib a %{outs[1]} %{ins}"
			}
		}
	end
)

definerule("sdccprogram",
	{
		srcs = { type="targets", default={} },
		deps = { type="targets", default={} },
	},
	function (e)
		local crt = "cpmtools+cpmcrt"

		return cprogram {
			name = e.name,
			srcs = e.srcs,
			deps = {
				crt,
				"cpmtools/libcpm+libcpm",
				e.deps,
			},
			_clibrary = sdcclibrary,
			commands = {
				"sdldz80 -nmjwz "
					.. "-i %{outs}.ihx "
					.. "-b _CODE=0x0100 "
					.. "-k /usr/share/sdcc/lib/z80 "
					.. "-l z80 "
					.. "-m "
					.. filenamesof(crt)[1] .. " "
					.. "%{ins} ",
				"makebin -p %{outs}.ihx - | dd status=none of=%{outs} bs=128 skip=2"
			}
		}
	end
)

definerule("file",
	{
		srcs = { type="targets", default={} },
	},
	function (e)
		return simplerule {
			name = e.name,
			ins = {},
			outs = e.srcs,
			commands = {}
		}
	end
)

definerule("diskimage",
    {
        diskdefs = { type="targets", default={ "diskdefs" } },
		format = { type="string" },
		bootfile = { type="targets" },
        map = { type="targets", default={} },
        deps = { type="targets", default={} },
    },
	function (e)
		local bootfile = filenamesof(e.bootfile)[1]
		local deps = { e.deps, e.diskdefs, e.bootfile }
		local commands = {}
		local outs = {}
		for dest, src in pairs(e.map) do
			if (type(dest) == "number") then
				error("all references must have a destination")
			else
				local f = filenamesof(src)
				if (#f ~= 1) then
					error("installable can only cope with targets emitting single files")
				end

				deps[#deps+1] = f
				commands[#commands+1] = "cpmcp -f "..e.format.." %{outs} "..f[1].." 0:"..dest
			end
		end

        return normalrule {
            name = e.name,
            outleaves = { e.name..".img" },
            ins = deps,
            commands = {
                "mkfs.cpm -f "..e.format.." -b "..bootfile.." %{outs}",
                commands
            }
        }
	end
)
