definerule("ackfile",
	{
		srcs = { type="targets" },
		deps = { type="targets", default={} },
		suffix = { type="string", default=".o" },
	},
	function (e)
		return cfile {
			name = e.name,
			srcs = e.srcs,
			deps = e.deps,
			suffix = e.suffix,
			commands = {
				"ack -mcpm -c -o %{outs} %{ins} %{hdrpaths} %{cpmcflags}"
			}
		}
	end
)

definerule("acklibrary",
	{
		srcs = { type="targets", default={} },
		hdrs = { type="targets", default={} },
		deps = { type="targets", default={} },
		suffix = { type="string", default=".o" },
	},
	function (e)
		return clibrary {
			name = e.name,
			srcs = e.srcs,
			hdrs = e.hdrs,
			deps = e.deps,
			suffix = e.suffix,
			_cfile = ackfile,
			commands = {
				"rm -f %{outs[1]}",
				"aal qc %{outs[1]} %{ins}"
			}
		}
	end
)

definerule("ackprogram",
	{
		srcs = { type="targets", default={} },
		deps = { type="targets", default={} },
	},
	function (e)
		return cprogram {
			name = e.name,
			srcs = e.srcs,
			deps = e.deps,
			_clibrary = acklibrary,
			commands = {
				"ack -mcpm -.c -o %{outs} %{ins} %{cpmldflags}"
			}
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
				if not f[1] then
					error("target must return at least one file (only the first of which will be installed)")
				end

				deps[#deps+1] = {f[1]}
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

definerule("binslice",
	{
		src = { type="targets" },
		start = { type="number" },
		length = { type="number" },
	},
	function (e)
		local f = filenamesof(e.src)
		if #f ~= 1 then
			error("binslice can only operate on a single file")
		end

		return normalrule {
			name = e.name,
			outleaves = { e.name..".img" },
			ins = e.src,
			commands = {
				"dd if=%{ins} of=%{outs} status=none bs=1 skip="..e.start.." count="..e.length
			}
		}
	end
)

