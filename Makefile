OBJ = .obj

all: bootdisk.img

bootdisk.img: \
		$(OBJ)/boottracks.img \
		diskdefs
	mkfs.cpm -f nc200cpm -b $(OBJ)/boottracks.img $@

$(OBJ)/boottracks.img: \
		$(OBJ)/supervisor/supervisor.img \
		$(OBJ)/cpm/ccp.img \
		$(OBJ)/cpm/bdos.img \
		$(OBJ)/cpm/bios.img
	cp $(OBJ)/supervisor/supervisor.img $@
	dd if=$(OBJ)/cpm/ccp.img of=$@ bs=1 seek=9K
	dd if=$(OBJ)/cpm/bdos.img of=$@ bs=1 seek=11K
	dd if=$(OBJ)/cpm/bios.img of=$@ bs=1 seek=14848

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

$(OBJ)/%.o: %.asm
	@mkdir -p $(dir $@)
	z80-unknown-coff-as -I$(OBJ) -g -o $@ $<

$(OBJ)/%.img: $(OBJ)/%.o utils/z80.ld
	@mkdir -p $(dir $@)
	z80-unknown-coff-ld -T utils/z80.ld -o $@ $<

$(OBJ)/%.img.inc: $(OBJ)/%.img $(OBJ)/objectify
	@mkdir -p $(dir $@)
	$(OBJ)/objectify < $< > $@

.SECONDARY:
