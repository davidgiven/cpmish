boot.img: supervisor.asm auto.img startup.inc tty.inc
	pasmo supervisor.asm supervisor.img

auto.img: auto.asm
	pasmo auto.asm auto.img
