OBJ = .obj

all: bootdisk.img

bootdisk.img: \
		$(OBJ)/boottracks.img \
		diskdefs
	mkfs.cpm -f nc200cpm -b $(OBJ)/boottracks.img $@
	#cpmcp -f nc200cpm $@ bbcbasic.com 0:bbcbasic.com

$(OBJ)/boottracks.img: \
		$(OBJ)/mammoth.img
	rm -f $@
	dd if=$< of=$@ bs=1K count=9
	dd if=$< of=$@ bs=1K seek=9 skip=58 count=6

$(OBJ)/mammoth.img: \
		utils/mammoth.ld \
		$(OBJ)/supervisor/supervisor.o \
		$(OBJ)/cpm/ccp.o \
		$(OBJ)/cpm/bdos.o \
		$(OBJ)/cpm/bios.o
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

$(OBJ)/font.inc: $(OBJ)/fontconvert 6x8font.png
	@mkdir -p $(dir $@)
	$(OBJ)/fontconvert 6x8font.png > $@

$(OBJ)/keyboard.inc: $(OBJ)/mkkeytab
	@mkdir -p $(dir $@)
	$(OBJ)/mkkeytab > $@

$(OBJ)/%.o: %.asm $(wildcard include/*.inc)
	@mkdir -p $(dir $@)
	z80-unknown-coff-as -I$(OBJ) -g -o $@ $<

$(OBJ)/%.img: $(OBJ)/%.o utils/z80.ld
	@mkdir -p $(dir $@)
	z80-unknown-coff-ld -T utils/z80.ld -o $@ $<

$(OBJ)/%.img.inc: $(OBJ)/%.img $(OBJ)/objectify
	@mkdir -p $(dir $@)
	$(OBJ)/objectify < $< > $@

.SECONDARY:
