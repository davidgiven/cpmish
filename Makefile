OBJ = .obj

supervisor.img: supervisor/supervisor.asm $(OBJ)/auto.img $(wildcard supervisor/*.inc)
	pasmo -I supervisor $< $@

$(OBJ)/auto.img: supervisor/auto.asm
	pasmo $< $@
