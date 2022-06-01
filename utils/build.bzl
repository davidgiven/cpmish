def unix2cpm(name, src):
    native.genrule(
        name = name,
        srcs = [ src ],
        outs = [ name + ".txt" ],
        tools = [ "//utils:unix2cpm" ],
        cmd = "$(location //utils:unix2cpm) $< > $@"
    )

def objectify(name, src):
    native.genrule(
        name = name,
        srcs = [ src ],
        outs = [ name + ".inc" ],
        tools = [ "//utils:objectify" ],
        cmd = "$(location //utils:objectify) $< > $@"
    )

# vim: ts=4 sw=4 et

