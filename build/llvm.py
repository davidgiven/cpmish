from build.ab import Rule, Targets, Target
from build.c import cprogram, cfile, clibrary


@Rule
def llvmcfile(
    self,
    name,
    srcs: Targets = [],
    deps: Targets = [],
    cflags=[],
    suffix=".o",
    toolchain=None,
    commands=["$(CC6502) -c -o {outs[0]} {ins[0]} $(CFLAGS6502) {cflags}"],
    label="CC6502",
):
    cfile(
        replaces=self,
        srcs=srcs,
        deps=deps,
        cflags=cflags,
        suffix=suffix,
        commands=commands,
        label=label,
    )


@Rule
def llvmprogram(
    self,
    name,
    srcs: Targets = [],
    deps: Targets = [],
    cflags=[],
    ldflags=[],
    commands=["$(CC6502) -o {outs[0]} {ins} {ldflags} $(LDFLAGS6502)"],
    label="CLINK6502",
):
    cprogram(
        replaces=self,
        srcs=srcs,
        deps=deps,
        cflags=cflags,
        ldflags=ldflags,
        commands=commands,
        label=label,
        cfilerule=llvmcfile,
        cfilekind="llvmprogram",
    )


@Rule
def llvmrawprogram(
    self,
    name,
    linkscript: Target,
    deps: Targets = [],
    commands=[
        "$(LD6502) -Map {outs[0]}.map -T {deps[-1]} -o {outs[0]} {ins} {ldflags}"
    ],
    label="LD6502",
    **kwargs
):
    cprogram(
        replaces=self,
        deps=deps + [linkscript],
        commands=commands,
        label=label,
        cfilerule=llvmcfile,
        cfilekind="llvmprogram",
        **kwargs
    )


@Rule
def llvmclibrary(
    name,
    self,
    commands=["rm -f {outs[0]} && $(AR6502) cqs {outs[0]} {ins}"],
    **kwargs
):
    clibrary(
        replaces=self,
        commands=commands,
        cfilerule=llvmcfile,
        label="LIB6502",
        **kwargs
    )
