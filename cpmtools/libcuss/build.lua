local libcuss_terminals = {
    "KAYPROII",
    "NC200",
	"BROTHEROP2",
	"BROTHER_WP2450DS",
	"SPECTRUM_PLUS_THREE",
	"SPECTRUM_NEXT",
}

for _, terminal in ipairs(libcuss_terminals) do
    acklibrary {
        name = "libcuss_"..terminal,
        srcs = { "./*.c" },
        hdrs = { "./*.h" },
        vars = {
            ["+cpmcflags"] = "-DLIBCUSS_"..terminal
        }
    }
end

definerule("libcussprogram",
    {
        srcs = { type="targets", default={} },
        deps = { type="targets", default={} },
    },
    function (e)
        for _, terminal in ipairs(libcuss_terminals) do
            ackprogram {
                name = e.name.."_"..terminal,
                srcs = e.srcs,
                deps = { e.deps, "cpmtools/libcuss+libcuss_"..terminal },
                vars = {
                    ["+cpmcflags"] = "-DLIBCUSS_"..terminal
                }
            }
        end
    end
)
