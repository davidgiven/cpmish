.PREFIX:
.PREFIX: .z80 .rel

SRCS = z8e.z80

all: z8e.z80
	cp 18e1.inc config.inc
	zxcc m80 -"=z8e.z80"
	zxcc l80 -"z8e,z8e/n/y/e"
	mv z8e.com 18e1.com
	cp 18e2.inc config.inc
	zxcc m80 -"=z8e.z80"
	zxcc l80 -"z8e,z8e/n/y/e"
	mv z8e.com 18e2.com
	cp z8e1.inc config.inc
	zxcc m80 -"=z8e.z80"
	zxcc l80 -"z8e,z8e/n/y/e"
	mv z8e.com z8e1.com
	cp z8e2.inc config.inc
	zxcc m80 -"=z8e.z80"
	zxcc l80 -"z8e,z8e/n/y/e"
	mv z8e.com z8e2.com

clean:
	rm -f *.com *.rel *.prn *.sym core *~
