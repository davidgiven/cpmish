OBJ = .obj

all: $(OBJ)/supervisor.img

$(OBJ)/supervisor.o: \
	$(OBJ)/font.inc \
	$(OBJ)/keyboard.inc \
	$(OBJ)/auto.img.inc \
	$(wildcard supervisor/*.inc)

$(OBJ)/%: utils/%.c
	@mkdir -p $(dir $@)
	gcc -g $< -o $@ -lm

$(OBJ)/font.inc: $(OBJ)/fontconvert 6x8font.png
	@mkdir -p $(dir $@)
	$(OBJ)/fontconvert 6x8font.png > $@

$(OBJ)/keyboard.inc: $(OBJ)/mkkeytab
	@mkdir -p $(dir $@)
	$(OBJ)/mkkeytab > $@

$(OBJ)/%.o: supervisor/%.asm
	@mkdir -p $(dir $@)
	z80-unknown-coff-as -g -o $@ $<

$(OBJ)/%.img: $(OBJ)/%.o utils/z80.ld
	@mkdir -p $(dir $@)
	z80-unknown-coff-ld -T utils/z80.ld -o $@ $<

$(OBJ)/%.img.inc: $(OBJ)/%.img $(OBJ)/objectify
	@mkdir -p $(dir $@)
	$(OBJ)/objectify < $< > $@

.SECONDARY:
