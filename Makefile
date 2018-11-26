OBJ = .obj

supervisor.img: supervisor/supervisor.asm $(OBJ)/font.inc $(OBJ)/auto.img $(OBJ)/ccp.img $(wildcard supervisor/*.inc)
	pasmo -I supervisor $< $@

$(OBJ)/fontconvert: utils/fontconvert.c
	gcc -g $< -o $@ -lm

$(OBJ)/font.inc: $(OBJ)/fontconvert 6x8font.png
	$(OBJ)/fontconvert 6x8font.png > $@

$(OBJ)/ccp.img: cpm/ccp.asm
	pasmo $< $@

$(OBJ)/auto.img: supervisor/auto.asm
	pasmo $< $@
