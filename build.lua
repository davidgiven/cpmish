vars.cflags = { "-g", "-Og" }
vars.ldflags = { "-lm" }
vars.cpmcflags = { "--opt-code-size" }
vars.cpmldflags = {}

diskimage {
    name = "nc200-img",
    format = "nc200cpm",
    bootfile = { "arch/nc200+bootfile" },
    map = {
        ["dump.com"] = "cpmtools+dump",
        ["stat.com"] = "cpmtools+stat",
        ["asm.com"] = "cpmtools+asm",
        ["copy.com"] = "cpmtools+copy",

        -- These binary tools are here temporarily and need to be removed ---
        -- we have no license for them.
        ["bbcbasic.com"] = "cpmtools/bbcbasic.com",
        ["ddt.com"] = "cpmtools/ddt.com",
        ["ed.com"] = "cpmtools/ed.com",
        ["pip.com"] = "cpmtools/pip.com",
        ["submit.com"] = "cpmtools/submit.com",
        ["xsub.com"] = "cpmtools/xsub.com",
    },
}
    
-- diskimage {
--     name = "osborne1-img",
--     format = "osborne1",
--     map = {
--         ["dump.com"] = "cpmtools+dump",
--         ["stat.com"] = "cpmtools+stat",
--         ["asm.com"] = "cpmtools+asm",

--         -- These binary tools are here temporarily and need to be removed ---
--         -- we have no license for them.
--         ["bbcbasic.com"] = "cpmtools/bbcbasic.com",
--         ["ddt.com"] = "cpmtools/ddt.com",
--         ["ed.com"] = "cpmtools/ed.com",
--         ["pip.com"] = "cpmtools/pip.com",
--         ["submit.com"] = "cpmtools/submit.com",
--         ["xsub.com"] = "cpmtools/xsub.com",
--     },
-- }
    
installable {
    name = "all",
    map = {
        ["emu"] = "utils/emu+emu",
        ["nc200.img"] = "+nc200-img",
    --    ["osborne1.img"] = "+osborne1-img"
    }
}
