include "third_party/zmac/build.lua"

normalrule {
    name = "keytab_inc",
    ins = { "arch/nc200/utils+mkkeytab" },
    outleaves = { "keytab.inc" },
    commands = {
        "%{ins} > %{outs}"
    }
}

normalrule {
    name = "font_inc",
    ins = {
        "arch/nc200/utils+fontconvert",
        "utils+6x7font",
    },
    outleaves = { "font.inc" },
    commands = {
        "%{ins} > %{outs}"
    }
}

zmac {
    name = "supervisor",
    srcs = { "./supervisor.z80" },
    deps = {
        "arch/nc200/include/*.lib",
        "./*.inc",
        "+keytab_inc",
        "+font_inc",
    },
}
