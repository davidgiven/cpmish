OBJ = .obj
CPMTOOLS = \
	$(wildcard cpmtools/*.com) \
	$(patsubst %.c,$(OBJ)/%.com,$(wildcard cpmtools/*.c))

ZMACINCLUDES = $(wildcard include/*.lib)

all: bootdisk.img

bootdisk.img: \
		$(OBJ)/boottracks.img \
		$(CPMTOOLS) \
		diskdefs
	mkfs.cpm -f nc200cpm -b $(OBJ)/boottracks.img $@
	cpmcp -f nc200cpm $@ $(CPMTOOLS) 0:

$(OBJ)/boottracks.img: \
		$(OBJ)/mammoth.cim
	rm -f $@
	dd if=$< of=$@ bs=256 count=36
	dd if=$< of=$@ bs=256 seek=36 skip=232 count=24

$(OBJ)/mammoth.cim: \
		$(OBJ)/ld80 \
		$(OBJ)/bios/bios.rel \
		$(OBJ)/supervisor/rootdir.rel \
		$(OBJ)/supervisor/relauto.rel \
		$(OBJ)/supervisor/startup.rel \
		$(OBJ)/supervisor/sirq.rel \
		$(OBJ)/supervisor/bpb1.rel \
		$(OBJ)/supervisor/bpb2.rel \
		$(OBJ)/supervisor/bootsig.rel \
		$(OBJ)/supervisor/fat.rel \
		$(OBJ)/supervisor/supervisor.rel \
		$(OBJ)/third_party/zcpr1/zcpr.rel \
		$(OBJ)/third_party/zsdos/zsdos-gp.rel
	@mkdir -p $(dir $@)
	$(OBJ)/ld80 \
		-O bin \
		-o $@ \
		-s $(patsubst %.cim,%.sym,$@) \
		-P 0000 $(OBJ)/supervisor/startup.rel \
		-P 000b $(OBJ)/supervisor/bpb1.rel \
		-P 0038 $(OBJ)/supervisor/sirq.rel \
		-P 01fe $(OBJ)/supervisor/bootsig.rel \
		-P 020b $(OBJ)/supervisor/bpb2.rel \
		-P 0400 $(OBJ)/supervisor/fat.rel \
		$(OBJ)/supervisor/supervisor.rel \
		-P 1000 $(OBJ)/supervisor/rootdir.rel \
		-P 1e00 $(OBJ)/supervisor/relauto.rel \
		-P e800 $(OBJ)/third_party/zcpr1/zcpr.rel \
		-P f000 $(OBJ)/third_party/zsdos/zsdos-gp.rel \
		-P fe00 $(OBJ)/bios/bios.rel

$(OBJ)/supervisor/supervisor.rel: \
	$(OBJ)/font.inc \
	$(OBJ)/keyboard.inc \
	$(wildcard supervisor/*.inc) \
	$(wildcard include/*.inc)

$(OBJ)/supervisor/relauto.rel: \
	$(OBJ)/supervisor/auto.cim.inc

$(OBJ)/%: utils/%.c
	@mkdir -p $(dir $@)
	gcc -g $< -o $@ -I. -lm

$(OBJ)/%.o: $(OBJ)/%.c
	@mkdir -p $(dir $@)
	gcc -g -c $< -o $@ -I.

$(OBJ)/%.o: %.c
	@mkdir -p $(dir $@)
	gcc -g -c $< -o $@ -I.

$(OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	gcc -g -c $< -o $@ -I.

$(OBJ)/%.c: %.y
	@mkdir -p $(dir $@)
	bison -y -o $@ $<

$(OBJ)/%.com: $(OBJ)/%.cim
	cp $< $@

$(OBJ)/%.rel: %.asm $(OBJ)/zmac
	@mkdir -p $(dir $@)
	$(OBJ)/zmac --zmac -m --rel7 -8 $< -o $@ -o $(patsubst %.rel,%.lst,$@) -Iinclude

$(OBJ)/%.rel: %.z80 $(OBJ)/zmac $(ZMACINCLUDES)
	@mkdir -p $(dir $@)
	$(OBJ)/zmac --zmac -m --rel7 $< -o $@ -o $(patsubst %.rel,%.lst,$@) -Iinclude

$(OBJ)/%.cim: %.z80 $(OBJ)/zmac $(ZMACINCLUDES)
	@mkdir -p $(dir $@)
	$(OBJ)/zmac --zmac -m $< -o $@ -o $(patsubst %.rel,%.lst,$@) -Iinclude

$(OBJ)/%.cim: %.asm $(OBJ)/zmac $(ZMACINCLUDES)
	@mkdir -p $(dir $@)
	$(OBJ)/zmac --zmac -m --rel7 -8 $< -o $@ -o $(patsubst %.rel,%.lst,$@) -Iinclude

ZMAC_OBJS = \
	$(OBJ)/third_party/zmac/mio.o \
	$(OBJ)/third_party/zmac/zi80dis.o \
	$(OBJ)/third_party/zmac/zmac.o
$(OBJ)/zmac: $(ZMAC_OBJS)
	@mkdir -p $(dir $@)
	gcc -g -o $@ $^
$(ZMAC_OBJS): $(wildcard third_party/zmac/*.h)

LD80_OBJS = \
	$(OBJ)/third_party/ld80/main.o \
	$(OBJ)/third_party/ld80/readobj.o \
	$(OBJ)/third_party/ld80/section.o \
	$(OBJ)/third_party/ld80/symbol.o \
	$(OBJ)/third_party/ld80/fixup.o \
	$(OBJ)/third_party/ld80/do_out.o \
	$(OBJ)/third_party/ld80/optget.o
$(OBJ)/ld80: $(LD80_OBJS)
	@mkdir -p $(dir $@)
	gcc -g -o $@ $^
$(LD80_OBJS): third_party/ld80/ld80.h

$(OBJ)/6x7font.png: 6x7font.bdf utils/bdftofont.sh
	@mkdir -p $(dir $@)
	./utils/bdftofont.sh 6x7font.bdf $@

$(OBJ)/font.inc: $(OBJ)/fontconvert $(OBJ)/6x7font.png
	@mkdir -p $(dir $@)
	$(OBJ)/fontconvert $(OBJ)/6x7font.png > $@

$(OBJ)/keyboard.inc: $(OBJ)/mkkeytab
	@mkdir -p $(dir $@)
	$(OBJ)/mkkeytab > $@

$(OBJ)/%.inc: $(OBJ)/% $(OBJ)/objectify
	@mkdir -p $(dir $@)
	$(OBJ)/objectify < $< > $@

$(OBJ)/%.h: $(OBJ)/% $(OBJ)/objectifyc
	@mkdir -p $(dir $@)
	$(OBJ)/objectifyc < $< > $@

$(OBJ)/emucpm.cim: \
		$(OBJ)/ld80 \
		$(OBJ)/third_party/zcpr1/zcpr.rel \
		$(OBJ)/utils/emu/biosbdos.rel
	@mkdir -p $(dir $@)
	$(OBJ)/ld80 \
		-O bin \
		-o $@.big \
		-P f700 $(OBJ)/third_party/zcpr1/zcpr.rel \
		-P ff00 $(OBJ)/utils/emu/biosbdos.rel
	dd if=$@.big of=$@ bs=1 skip=63232

EMU_OBJS = \
	$(OBJ)/utils/emu/biosbdos.o \
	$(OBJ)/utils/emu/emulator.o \
	$(OBJ)/utils/emu/fileio.o \
	$(OBJ)/utils/emu/main.o
$(OBJ)/emu: $(EMU_OBJS)
	@mkdir -p $(dir $@)
	gcc -g -o $@ $^ -lz80ex -lz80ex_dasm -lreadline

$(EMU_OBJS): utils/emu/globals.h
$(OBJ)/utils/emu/biosbdos.o: .obj/emucpm.cim.h

#$(OBJ)/cpmtools/%.crl: cpmtools/%.c \
#	$(OBJ)/emu \
#	$(OBJ)/third_party/bdsc/cc.com \
#	$(OBJ)/third_party/bdsc/cc2.com
#	@mkdir -p $(dir $@)
#	cp $< $(dir $@)
#	$(OBJ)/emu \
#		-p B=$(OBJ)/third_party/bdsc \
#		-p C=$(dir $@) \
#		-- \
#		$(OBJ)/third_party/bdsc/cc.com \
#		c:$(notdir $<) \
#		-ab -dc
#	
#$(OBJ)/cpmtools/%.com: $(OBJ)/cpmtools/%.crl \
#	$(OBJ)/emu \
#	$(OBJ)/third_party/bdsc/clink.com \
#	$(OBJ)/third_party/bdsc/c.ccc
#	@mkdir -p $(dir $@)
#	$(OBJ)/emu \
#		-p B=$(OBJ)/third_party/bdsc \
#		-p C=$(dir $@) \
#		-p D=$(dir $<) \
#		-- \
#		$(OBJ)/third_party/bdsc/clink.com \
#		d:$(notdir $<) \
#		-o c:$(notdir $@) \
#		-cb
#
#$(OBJ)/third_party/bdsc/c.ccc: $(OBJ)/third_party/bdsc/c.cim
#	@mkdir -p $(dir $@)
#	cp $< $@

$(OBJ)/cpmtools/%.obj: cpmtools/%.asm
	@mkdir -p $(dir $@)
	sdasz80 -g -o $@ $<

$(OBJ)/cpmtools/%.obj: cpmtools/%.c cpmtools/libcpm.h
	@mkdir -p $(dir $@)
	sdcc -mz80 -c --opt-code-size -o $@ $<

LIBCPM_SRCS = $(wildcard cpmtools/libcpm/*.asm)
LIBCPM_OBJS = $(patsubst %.asm,$(OBJ)/%.obj,$(LIBCPM_SRCS))

$(OBJ)/cpmtools/libcpm.lib: $(LIBCPM_OBJS)
	@mkdir -p $(dir $@)
	rm -f $@
	sdcclib a $@ $^

$(OBJ)/cpmtools/%.com: $(OBJ)/cpmtools/%.obj $(OBJ)/cpmtools/cpmcrt.obj $(OBJ)/cpmtools/libcpm.lib
	@mkdir -p $(dir $@)
	sdldz80 -nmjwz \
		-i $@.ihx \
		-b _CODE=0x0100 \
		-k /usr/share/sdcc/lib/z80 \
		-l z80 \
		-k $(OBJ)/cpmtools \
		-l libcpm \
		-m \
		$(OBJ)/cpmtools/cpmcrt.obj \
		$<
	makebin -p $@.ihx - | dd of=$@ bs=128 skip=2

clean:
	rm -rf .obj
	rm -f bootdisk.img

.SECONDARY:
