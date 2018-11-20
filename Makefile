boot.img: bootblock.asm auto.img startup.inc tty.inc
	pasmo bootblock.asm boot.img

auto.img: auto.asm
	pasmo auto.asm auto.img
