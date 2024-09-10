from build.c import cprogram

cprogram(name="fontconvert", srcs=["./fontconvert.c"], deps=["utils+libbdf"])
