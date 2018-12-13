VERSION = 0.5
CFLAGS = -g -Wall -Wstrict-prototypes -DVERSION=\"$(VERSION)\"
CFLAGS := $(CFLAGS) -DDEBUG

.SUFFIXES: .pod .1 .html .ps

OBJS = main.o readobj.o section.o symbol.o fixup.o do_out.o optget.o
MANPAGES = ld80.1
PSFILES = ld80.ps

ld80:		$(OBJS)
		gcc -g -o ld80 $(OBJS)

main.o:		ld80.h

readobj.o:	ld80.h

section.o:	ld80.h

fixup.o:	ld80.h

symbol.o:	ld80.h

do_out.o:	ld80.h

optget.o:	ld80.h

clean:
		rm -f *.o pod2html-*cache

distclean:	clean
		rm -f $(MANPAGES) $(PSFILES) ld80

tar:
		tar -cvzf /tmp/ld80-$(VERSION).tgz *.c *.h ld80* Makefile

man:		ld80.1

ps:		ld80.ps

.pod.1:
		pod2man --center=' ' --release='$* $(VERSION)' \
			--section=1 $*.pod > $*.1

.pod.html:
		pod2html --noindex $*.pod | sed 's/<HR>//g' > $*.html

.1.ps:
		troff -man $*.1 | grops > $*.ps
