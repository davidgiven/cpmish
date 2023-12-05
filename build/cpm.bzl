load("//third_party/zmac:build.bzl", "zmac")
load("//third_party/ld80:build.bzl", "ld80")

def z80test(name, srcs, deps=[]):
    zmac(
        name = name + "_zmac",
        srcs = srcs
    )

    ld80(
        name = name + "_cim",
        address = 0x0100,
        objs = {
            0x0100: [ ":" + name + "_zmac" ] + deps
        }
    )

    native.sh_test(
        name = name + "_test",
        size = "small",
        srcs = [ "//build:cpmtest.sh" ],
        data = [
            "//utils/emu",
            ":" + name + "_cim"
        ],
        args = [ "$(location //utils/emu)", "$(location :" + name + "_cim)" ],
    )


#definerule("z80test",
#	{
#		srcs = { type="targets" },
#		deps = { type="targets", default={} },
#    },
#    function (e)
#        local rel = zmac {
#            name = e.name.."_zmac",
#            srcs = e.srcs,
#            deps = e.deps,
#        }
#
#        local cim = ld80 {
#            name = e.name.."_cim",
#            srcs = { "-P0x0100", rel }
#        }
#
#        local com = bintocom {
#            name = e.name.."_com",
#            srcs = { cim }
#        }
#
#        normalrule {
#            name = e.name.."_test",
#            ins = {
#                "utils/emu+emu",
#                com
#            },
#            outleaves = { "log" },
#            commands = {
#                "%{ins[1]} %{ins[2]} > %{outs} && test ! -s %{outs}"
#            }
#        }
#    end
#)


def cpm_addresses(name, top_of_memory=0x10000, bios_size=None):
    bdos_size = 3584 # fixed
    ccp_size = 2048  # fixed
    bbase = top_of_memory - bios_size
    fbase = bbase - bdos_size
    cbase = fbase - ccp_size

    native.genrule(
        name = name,
        srcs = [],
        outs = [ name + ".lib" ],
        cmd = " && ".join([
            "echo BIOS_SIZE = %d > $@" % bios_size,
            "echo BDOS_SIZE = %d >> $@" % bdos_size,
            "echo CCP_SIZE = %d >> $@" % ccp_size,
            "echo CPM_SIZE = %d >> $@" % (bios_size + bdos_size + ccp_size),
            "echo BBASE = %d >> $@" % bbase,
            "echo FBASE = %d >> $@" % fbase,
            "echo CBASE = %d >> $@" % cbase,
        ])
	)

    return (cbase, fbase, bbase)

def binslice(name, src, start, length=None):
    start = start + 1
    if length == None:
        native.genrule(
            name = name,
            srcs = [ src ],
            outs = [ name + ".img" ],
            cmd = "tail -c+%d $< > $@" % (start)
        )
    else:
        native.genrule(
            name = name,
            srcs = [ src ],
            outs = [ name + ".img" ],
            cmd = "tail -c+%d $< | head -c%d > $@" % (start, length)
        )
        
def diskimage(name, format, bootfile, map):
    out = name + ".img"
    native.genrule(
        name = name,
        srcs = [ bootfile, "//:diskdefs" ] + map.values(),
        outs = [ out ],
        cmd = " && ".join([
            "mkfs.cpm -f {} -b $(location {}) $@".format(format, bootfile)
        ] + [
            "cpmcp -f {} $@ $(location {}) 0:{}".format(format, map[name], name)
            for name in map
        ] + [
            "(cpmcp -f {} $@ /dev/zero 0:padding 2> /dev/null) || true".format(format),
            "cpmrm -f {} $@ 0:padding".format(format)
        ])
    )

# vim: sw=4 ts=4 et

