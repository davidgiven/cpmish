OBJ = .obj
CPMTOOLS = $(wildcard cpmtools/*)

all: bootdisk.img

bootdisk.img: \
		$(OBJ)/boottracks.img \
		$(CPMTOOLS) \
		diskdefs
	mkfs.cpm -f nc200cpm -b $(OBJ)/boottracks.img $@
	cpmcp -f nc200cpm $@ $(CPMTOOLS) 0:

 $(OBJ)/boottracks.img: \
		$(OBJ)/mammoth.img
	rm -f $@
	dd if=$< of=$@ bs=1K count=9
	dd if=$< of=$@ bs=1K seek=9 skip=58 count=6

$(OBJ)/supervisor.img: \
		utils/mammoth.ld \
		$(OBJ)/supervisor/supervisor.o
	@mkdir -p $(dir $@)
	z80-unknown-coff-ld -T utils/mammoth.ld -o $@ $(filter %.o, $^)

$(OBJ)/supervisor/supervisor.o: \
	$(OBJ)/font.inc \
	$(OBJ)/keyboard.inc \
	$(OBJ)/supervisor/auto.img \
	$(wildcard supervisor/*.inc) \
	$(wildcard include/*.inc)

$(OBJ)/%: utils/%.c
	@mkdir -p $(dir $@)
	gcc -g $< -o $@ -lm

$(OBJ)/%.o: %.c
	@mkdir -p $(dir $@)
	gcc -g -c $< -o $@

$(OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	gcc -g -c $< -o $@

$(OBJ)/%.o: $(OBJ)/%.c
	@mkdir -p $(dir $@)
	gcc -g -c $< -o $@ -Izmac

$(OBJ)/%.c: %.y
	@mkdir -p $(dir $@)
	bison -y -o $@ $<

$(OBJ)/%.com: $(OBJ)/%.cim
	cp $< $@

$(OBJ)/%.rel: %.asm $(OBJ)/zmac
	@mkdir -p $(dir $@)
	$(OBJ)/zmac -8 $< -o $@ -Iinclude

$(OBJ)/%.cim: %.z80 $(OBJ)/zmac
	@mkdir -p $(dir $@)
	$(OBJ)/zmac $< -o $@ -Iinclude

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

$(OBJ)/%.o: %.asm $(wildcard include/*.inc)
	@mkdir -p $(dir $@)
	z80-unknown-coff-as -I$(OBJ) -g -o $@ $<

$(OBJ)/%.img: $(OBJ)/%.o utils/z80.ld
	@mkdir -p $(dir $@)
	z80-unknown-coff-ld -T utils/z80.ld -o $@ $<

$(OBJ)/%.inc: $(OBJ)/% $(OBJ)/objectify
	@mkdir -p $(dir $@)
	$(OBJ)/objectify < $< > $@

.SECONDARY:
