def unix2cpm(name, src):
    native.genrule(
        name = name,
        srcs = [ src ],
        outs = [ name + ".txt" ],
        tools = [ "//utils:unix2cpm" ],
        cmd = "$(location //utils:unix2cpm) < $< > $@"
    )

def objectify(name, src):
    native.genrule(
        name = name,
        srcs = [ src ],
        outs = [ name + ".inc" ],
        tools = [ "//utils:objectify" ],
        cmd = "$(location //utils:objectify) < $< > $@"
    )

def objectifyc(name, src):
    native.genrule(
        name = name,
        srcs = [ src ],
        outs = [ name + ".h" ],
        tools = [ "//utils:objectifyc" ],
        cmd = "$(location //utils:objectifyc) < $< > $@"
    )

# vim: ts=4 sw=4 et

