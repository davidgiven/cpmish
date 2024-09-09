from build.c import cprogram

cprogram(name="mkkeytab", srcs=["./mkkeytab.c"])
cprogram(name="fontconvert", srcs=["./fontconvert.c"], deps=["utils+libbdf"])
