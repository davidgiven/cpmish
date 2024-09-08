export OBJ = .obj

.PHONY: all
all: +all

TARGETS = +all
include build/ab.mk
