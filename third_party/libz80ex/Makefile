# Z80ex Makefile
# (for GNU make)
#

#################################################################
# You may tune these values to fit your setup:
#################################################################
INSTALL_PREFIX := /usr/local
TOOLS_PREFIX :=

CC := $(TOOLS_PREFIX)gcc 
LINKER := $(TOOLS_PREFIX)gcc
AR := $(TOOLS_PREFIX)ar

ALL_CFLAGS := -fPIC -fno-common -ansi -pedantic -Wall -pipe -O2 -I. -I./include 

#endianness (one of: WORDS_LITTLE_ENDIAN, WORDS_BIG_ENDIAN)
ENDIANNESS := WORDS_LITTLE_ENDIAN
#ENDIANNESS := WORDS_BIG_ENDIAN

#fast and rough opcode step emulation mode (0 - off, 1 - on)
OPSTEP_FAST_AND_ROUGH := 0


#################################################################
# Do not change these:
#################################################################
PROJ := z80ex
EMU := libz80ex
DASM := libz80ex_dasm
API_REVISION := 1
VERSION_MAJOR:=1
VERSION_MINOR:=21
RELEASE_TYPE :=
VERSION_STR:= ${API_REVISION}.${VERSION_MAJOR}.${VERSION_MINOR}${RELEASE_TYPE}

OS=${shell uname -s}

ALL_CFLAGS += -D${ENDIANNESS} -DZ80EX_VERSION_STR=${VERSION_STR} -DZ80EX_API_REVISION=${API_REVISION} -DZ80EX_VERSION_MAJOR=${VERSION_MAJOR} -DZ80EX_VERSION_MINOR=${VERSION_MINOR} -DZ80EX_RELEASE_TYPE=${RELEASE_TYPE} 

ifneq (${OPSTEP_FAST_AND_ROUGH},0)
ALL_CFLAGS += -DZ80EX_OPSTEP_FAST_AND_ROUGH
endif

c_files := z80ex.c z80ex_dasm.c

%.o : %.c
	${CC} ${ALL_CFLAGS} ${CFLAGS} -c -o $@ $<	

.PHONY : all
all:: static shared

z80ex.o: include/z80ex.h include/z80ex_common.h ptables.c typedefs.h macros.h opcodes/opcodes_base.c\
opcodes/opcodes_dd.c opcodes/opcodes_fd.c opcodes/opcodes_cb.c\
opcodes/opcodes_ed.c opcodes/opcodes_ddcb.c opcodes/opcodes_fdcb.c

z80ex_dasm.o: include/z80ex_dasm.h include/z80ex_common.h opcodes/opcodes_dasm.c

clean:
	rm -f *.o
	rm -f ./lib/*
	rm -rf ./z80ex-${VERSION_STR}.tar.gz

static: z80ex.o z80ex_dasm.o
	${AR} rs ./lib/${EMU}.a z80ex.o
	${AR} rs ./lib/${DASM}.a z80ex_dasm.o
	
shared: z80ex.o z80ex_dasm.o
ifeq (${OS},Darwin)
	${LINKER} -dynamiclib -compatibility_version ${API_REVISION} -current_version ${VERSION_STR} -install_name ${INSTALL_PREFIX}/lib/${EMU}.${API_REVISION}.dylib -o ./lib/${EMU}.${VERSION_STR}.dylib z80ex.o
	${LINKER} -dynamiclib -compatibility_version ${API_REVISION} -current_version ${VERSION_STR} -install_name ${INSTALL_PREFIX}/lib/${DASM}.${API_REVISION}.dylib -o ./lib/${DASM}.${VERSION_STR}.dylib z80ex_dasm.o
else
	${LINKER} -shared -Wl,-soname,${EMU}.so.${API_REVISION} -o ./lib/${EMU}.so.${VERSION_STR} z80ex.o
	${LINKER} -shared -Wl,-soname,${DASM}.so.${API_REVISION} -o ./lib/${DASM}.so.${VERSION_STR} z80ex_dasm.o	
endif
	
install:
	install -d ${INSTALL_PREFIX}/lib
	install ./lib/* ${INSTALL_PREFIX}/lib
	install -d ${INSTALL_PREFIX}/include/z80ex
	install -m 0664 ./include/* ${INSTALL_PREFIX}/include/z80ex
ifeq (${OS},Darwin)
	ln -sf ${EMU}.${VERSION_STR}.dylib ${INSTALL_PREFIX}/lib/${EMU}.${API_REVISION}.dylib
	ln -sf ${EMU}.${VERSION_STR}.dylib ${INSTALL_PREFIX}/lib/${EMU}.dylib
	ln -sf ${DASM}.${VERSION_STR}.dylib ${INSTALL_PREFIX}/lib/${DASM}.${API_REVISION}.dylib
	ln -sf ${DASM}.${VERSION_STR}.dylib ${INSTALL_PREFIX}/lib/${DASM}.dylib	
else
	ln -sf ${EMU}.so.${VERSION_STR} ${INSTALL_PREFIX}/lib/${EMU}.so.${API_REVISION}
	ln -sf ${EMU}.so.${VERSION_STR} ${INSTALL_PREFIX}/lib/${EMU}.so
	ln -sf ${DASM}.so.${VERSION_STR} ${INSTALL_PREFIX}/lib/${DASM}.so.${API_REVISION}
	ln -sf ${DASM}.so.${VERSION_STR} ${INSTALL_PREFIX}/lib/${DASM}.so		
endif	

dist: clean
	rm -rf ./${PROJ}-${VERSION_STR}
	ln -s ./ ./${PROJ}-${VERSION_STR}
	tar --exclude-vcs --exclude obsolete --exclude ${PROJ}-${VERSION_STR}/${PROJ}-${VERSION_STR} --exclude ${PROJ}-${VERSION_STR}/${PROJ}-${VERSION_STR}.tar.gz -hcf - ./${PROJ}-${VERSION_STR}/ | gzip -f9 > ${PROJ}-${VERSION_STR}.tar.gz
	rm -rf ./${PROJ}-${VERSION_STR}

#EOF
