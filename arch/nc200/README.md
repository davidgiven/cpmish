Platform: the Amstrad NC200
===========================

The Amstrad NC200 is a late-era Z80 laptop. It's pretty high specification
for a Z80, with a fast processor, 128kB of RAM (expandable up to a megabyte)
720kB floppy disk drive, rather nice monochrome screen, and decent battery
life.

It's not really supposed to boot from floppy but [there's a backdoor in the
ROM-based software
suite](http://cowlark.com/2017-12-04-nc200-reverse-engineering) which will
seamlessly run programs from disk if you use the right keyboard combination.

What you get with this port:

- standard PC 720kB floppy disks (with the CP/M file system, of course, but
they're writeable from ordinary PC drives with normal sector skew)
- most of an ADM-3a / Kaypro II terminal emulator supporting 80x18 text
- a gigantic 60kB TPA
- an interrupt-driven keyboard
- a massive disk cache (because the NC200 has 128kB of RAM, CP/M uses 64kB,
and I had to do _something_ with the rest)
- bugs

What you don't get:

- power management (if you turn the machine off, all state is lost)
- memory card support (I'll get round to doing it... ~~once I find my SRAM
  card~~ eventually)
- repeat key
- blinking cursor
- printer or serial port support
- sysgen, format etc
- no bugs


How to use it
-------------

Format a 720kB floppy disk on a PC and `dd` the `nc200.img` file onto it.

Insert the disk into the machine's drive, power on, and press Code+Q. CP/Mish
will boot.

**Big warning:** CP/Mish uses _all_ the memory in your system; if you have
files stored in RAM, **you will lose them**. The entire state of the machine
is destroyed. You have been warned!

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

You can replace the CCP and BIOS with your own if you like, although you do
need to do this from Linux (because I haven't written the CP/M tools for this
yet).

To replace the BDOS:

- create a standard 3.5kB BDOS image assembled at 0xf000.
- do `dd if=my-custom-bdos.bin of=/dev/fd0 bs=1 seek=11264`

To replace the CCP:

- create a standard 2kB CCP image assembled at 0xe800.
- do `dd if=my-custom-ccp.bin of=/dev/fd0 bs=1 seek=9216`

Then, inserting the disk into the NC200 and warm starting CP/M will load your
new BDOS and CCP. (You don't even have to turn it off.)


Technical details
-----------------

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

Everything here was written by me, David Given, and is covered under the
terms of the whole CP/Mish project. See the documentation in the project root
for more information.
