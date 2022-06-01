def bison(name, src):
    CFILE = "%s.c" % name
    HFILE = "%s.h" % name
    native.genrule(
        name = name,
        srcs = [src],
        outs = [CFILE, HFILE],
        cmd = "bison --defines=$(location {}) --output=$(location {}) $<".format(HFILE, CFILE)
    )

def gperf(name, src, hash_function_name="hash", lookup_function_name="name"):
    HFILE = "%s.h" % name
    native.genrule(
        name = name,
        srcs = [src],
        outs = [HFILE],
        cmd = "gperf -o -E --constants-prefix={} -H{} -N{} -DptlC --output-file=$@ $<".format(
            name, hash_function_name, lookup_function_name)
    )

# vim: ts=4 sw=4 et

