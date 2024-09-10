from build.ab import simplerule, Rule, TargetsMap, Target, filenameof


def cpm_addresses(name, top_of_memory=0x10000, bios_size=None):
    bdos_size = 3584  # fixed
    ccp_size = 2048  # fixed
    bbase = top_of_memory - bios_size
    fbase = bbase - bdos_size
    cbase = fbase - ccp_size

    simplerule(
        name=name,
        ins=[],
        outs=[f"={name}.lib"],
        commands=[
            "echo BIOS_SIZE = %d > $@" % bios_size,
            "echo BDOS_SIZE = %d >> $@" % bdos_size,
            "echo CCP_SIZE = %d >> $@" % ccp_size,
            "echo CPM_SIZE = %d >> $@" % (bios_size + bdos_size + ccp_size),
            "echo BBASE = %d >> $@" % bbase,
            "echo FBASE = %d >> $@" % fbase,
            "echo CBASE = %d >> $@" % cbase,
        ],
        label="ADDRESSES",
    )

    return (cbase, fbase, bbase)


def binslice(name, src, start, length=None):
    if length == None:
        cmd = "dd status=none if=$< of=$@ bs=1 skip=%d" % (start)
    else:
        cmd = "dd status=none if=$< of=$@ bs=1 skip=%d count=%d" % (
            start,
            length,
        )

    simplerule(
        name=name,
        ins=[src],
        outs=[f"={name}.img"],
        commands=[cmd],
        label="BINSLICE",
    )


@Rule
def diskimage(self, name, format, bootfile: Target, map: TargetsMap, size=None):
    simplerule(
        replaces=self,
        ins=[bootfile, "diskdefs"] + list(map.values()),
        outs=[f"={name}.img"],
        commands=[f"mkfs.cpm -f {format} -b {{ins[0]}} {{outs[0]}}"]
        + ([f"truncate -s {size} {{outs[0]}}"] if size else [])
        + [
            f"cpmcp -f {format} {{outs[0]}} {filenameof(v)} 0:{k}"
            for k, v in map.items()
        ]
        + [
            f"(cpmcp -f {format} {{outs[0]}} /dev/zero 0:padding 2> /dev/null) || true",
            f"cpmrm -f {format} {{outs[0]}} 0:padding",
        ],
        label="DISKIMAGE",
    )
