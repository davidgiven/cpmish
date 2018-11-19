boot.img: bootblock.asm auto.img
	pasmo bootblock.asm boot.img

auto.img: auto.asm
	pasmo auto.asm auto.img
