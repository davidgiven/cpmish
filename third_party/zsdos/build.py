from third_party.zmac.build import zmac

zmac(name="zsdos", src="./zsdos-gp.z80", deps=["./zsdos.lib"])
