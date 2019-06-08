CP/M for the Amstrad NC200
==========================

What?
-----

This is a port of the 1977 CP/M operating system to the Amstrad NC200 Z80-based laptop
computer. It installs and boots from floppy and allows you to use the machine
as a single-disk workstation.

What you get:

- ZSDOS and ZCPR1
- standard PC 720kB floppy disks (with the CP/M file system, of course, but
they're writeable from ordinary PC drives with normal sector skew)
- a diskdefs file allowing you to use cpmtools with the same
- most of an ADM-3a / Kaypro II terminal emulator supporting 80x18 text
- a gigantic 60kB TPA
- an interrupt-driven keyboard
- a massive disk cache (because the NC200 has 128kB of RAM, CP/M uses 64kB,
and I had to do _something_ with the rest)
- bugs

What you don't get:

- power management (if you turn the machine off, all state is lost)
- memory card support (I'll get round to doing it... ~~once I find my SRAM card~~ eventually)
- repeat key
- blinking cursor
- printer or serial port support
- sysgen, format etc
- no bugs

Okay, disclosure time: this isn't really CP/M, as in Digital Research CP/M.
While the Digital Research BDOS and CCP work perfectly well on the NC200,
they're only legally distributable from one site, [The Unofficial CP/M
Site](http://www.cpm.z80.de/), and so can't be included here. Instead, what
you get are very slightly modified versions of the venerable ZSDOS and ZCPR1
alternative BDOS and CCP, which are legally distributable. They're better,
anyway. There is, in fact, no Digital Research code here at all.

If you want the original experience, download the Digital Research BDOS and
CCP, assemble them at 0xf000 and 0xe800 respectively, and use the
instructions below to patch them into the boot image. They work fine!

Why?
----

CP/M is a great operating system: it's the smallest you can make a useful
desktop disk-based operating system. And useful it was; it dominated the
personal computer market until the PC came along.

In fact, watch this:

[![Video of me doing stuff on an NC200](http://img.youtube.com/vi/FGWshrMZcCc/0.jpg)](https://www.youtube.com/watch?v=FGWshrMZcCc)

The NC200 is a great machine for CP/M, having a high-capacity (for the era)
floppy disk drive, lots of RAM, and memory banking.

Where?
------

- [Get the latest
release](https://github.com/davidgiven/nc200cpm/releases/latest) if you want
precompled binaries!

- [Check out the GitHub repository](http://github.com/davidgiven/nc200cpm)
and build from source. (Alternatively, you can download a source snapshot
from [the latest
release](https://github.com/davidgiven/nc200cpm/releases/latest), but I
suggect the GitHub repositories better because I don't really intend to make
formal releases often.) Build instructions as in the README.

- [Ask a question by creating a GitHub
issue](https://github.com/davidgiven/nc200cpm/issues/new), or just email me
directly at [dg@cowlark.com](mailto:dg@cowlark.com). (But I'd prefer you
opened an issue, so other people can see them.)


How?
----

### Installation and running instructions

The easy way:

- Download the bootable disk image; dd it onto a 720kB floppy disk; insert
into your NC200 and power on; then press Function+R to boot.

The hard way:

- type `make`, which will generate a bootable disk image; proceed as above.
You'll need sdcc, netpbm and cpmtools (and, probably, a Linux system).

**Big warning:** CP/M uses _all_ the memory in your system; if you have files
*stored in RAM, **you will lose them**. The entire state of the machine is
*destroyed. You have been warned!

Also, **read this before you start**:

- because of the disk cache, it's only safe to change disks when you're
looking at the command prompt. After changing disks, you _must_ do `^C` (even
if you're using something like ZSDOS which claims to autodetect this).
Otherwise you may corrupt your new disk.

- because of the disk cache, it's only safe to power off when you're looking
at the cursor; otherwise data may not have been written all the way to disk.

- format disks on a PC, not on the NC200 itself --- for some reason trying to
boot from an NC200-formatted disk hard crashes the machine. Don't know why;
it's probably due to the non-standard sector skew.

This is a traditional CP/M port with the usual interfaces between the CCP,
BDOS and BIOS. You can replace the CCP and BIOS with your own if you like,
although you do need to do this from Linux (because I haven't written the
CP/M tools for this yet).

To replace the BDOS:

- create a standard 3.5kB BDOS image assembled at 0xf000.
- do `dd if=my-custom-bdos.bin of=/dev/fd0 bs=1 seek=11264`

To replace the CCP:

- create a standard 2kB CCP image assembled at 0xe800.
- do `dd if=my-custom-ccp.bin of=/dev/fd0 bs=1 seek=9216`

Then, inserting the disk into the NC200 and warm starting CP/M will load your
new BDOS and CCP. (You don't even have to turn it off.)

### Technical details

The NC200 has a simple MMU allowing any 16kB bank of RAM to be mapped
anywhere in memory, and this port exploits this to keep most of the BIOS code
out of user space --- hence the large TPA. In fact, the BIOS is a simple stub
which just banks in the code which does the real work (the Supervisor) and
calls that.

The Supervisor is 8kB long and shares a 16kB bank with the video memory.
However, it's only mapped in when it's actually doing something, so user code
will never know it exists.

Given that four banks provide the 64kB of CP/M userspace, and one bank
contains the Supervisor, this leaves three banks spare; these are used for a
gigantic 48kB LRU disk cache which is used for deblocking and buffering. The
Supervisor actually reads and writes entire 9kB tracks as a unit, hence the
good disk performance, and the CP/M sector accesses just read and write from
the buffer. It's simple and very fast.

The terminal emulator is ADM-3a with some Kaypro II extensions, using a
custom (drawn by me!) 6x7 font to allow 80x18 characters of text. This
appears to be a relatively common choice and most software I've tried (Turbo
Pascal, VDE etc) works. Sadly, games tend not to work due to the non-standard
screen size.

Who?
----

Most of this software was written entirely (except for the exceptions noted
below) by me, David Given. Feel free to send me email at
[dg@cowlark.com](mailto:dg@cowlark.com). You may also [like to visit my
website](http://cowlark.com); there may or may not be something interesting
there.

However, the CP/M 2.2 BDOS and CCP, plus the standard utilities, were _not_
written by me. See below for the full licensing.

License?
--------

Everything here _except_ for the contents of third_party is © 2018 David
Given, and is open source software available [under the 2-clause BSD
license](https://github.com/davidgiven/nc200cpm/blob/master/COPYING).
Simplified summary: do what you like with it, just don't claim you wrote it.

Exception: the contents of the `third_party/zcpr1` directory contains a
(modified) copy of the ZCPR1 CCP replacement, written by the CCP-GROUP, which
is in the public domain. See the
[third_party/zcpr1/COPYING](third_party/zcpr1/COPYING) file for more details.

Exception: the contents of the `third_party/zmac` directory contains a
(modified) copy of the ZMAC macro assembler, written by George Phillips,
Thierry Join, Mark Rison, Russell Marks, Colin Kelley, John Providenza and
Bruce Norskog (some time in 1978!) --- and probably others. To the best of my
knowledge this is in the public domain. See the
[third_party/zmac/COPYING](third_party/zmac/COPYING) file for more details.

Exception: the contents of the `third_party/ld80` directory contains a
(modified) copy of the LD80 macro assembler, written by (as far as I know)
George Philips and Gabor Kiss. It is in the public domain.

Exception: the contents of the `third_party/zsdos` directory contains a
(modified) copy of the ZSDOS CP/M BDOS clone, written by lots of people but
mainly Cameron W. Cotrill and Harold F. Bower. This is available under the
terms of the General Public License version 1. See the
[third_party/zsdos/COPYING](third_party/zsdos/COPYING) file for more details.

**Important note:** Because of the presence of the GPLd ZSDOS, this
distribution as a whole must be treated as if it's distributable under the
terms of the GPL. If that's a problem, swap out ZSDOS with a different BDOS
(it's easy).
