Platform: the Brother WP-2450DS (or similar) word processor
==========================================================

The WP-2450DS is a 1983 word processing appliance: a simple Z180 computer with
a cool portrait CRT and a 3.5" floppy disk drive built in to a daisywheel
typewriter chassis. It's got at least 64kB of RAM and a 728x240 pixel screen,
with an excellent keyboard attached.

It's not really supposed to boot from floppy but Brother did release a small
amount of software for it, including
[Tetris](https://youtube.com/watch?v=3lSHfCdPRgw). There's a MAME port, on
which I have cpmish working (I haven't been able to verify it on real hardware
yet).

Luckily, this series of typewriters (all variations on the WP-75) use standard
720kB DOS floppies, unlike some of their other models. So, cpmish disks can be
easily created on a PC drive.

What you get with this port:

- about 702kB of storage on a 720kB GCR disk (I have to reserve two tracks for
  a FAT filesystem and the CP/M system track to boot from)
- most of an ADM-3a / Kaypro II terminal emulator supporting 90x30 text
- a respectable 55kB TPA
- a non-interrupt driven keyboard which drops keypresses if the machine's busy
- no cursor
- bugs

What you don't get:

- repeat key
- support for the printer
- sysgen, format etc
- no bugs

As you may gather, it's all a bit fragile.


How to use it
-------------

Build cpmish.

Use `dd` or your favourite disk imager to write the `wp2450ds.img` file to a DD
3.5" floppy. You're probably best off formatting this on the machine itself, to
get optimal interleave for the very slow machine.

Insert the disk into the machine's drive, power on, and pick 'Disk Application'
from the main menu. Then execute `CPMISH.APL`. After a few seconds the screen
will go blank, and after a few more seconds cpmish should start.

**Big warning:** if you save any Brother files onto the disk, you'll
irrevocably corrupt the CP/M filesystem. Don't do this.

You can replace the CCP and BIOS with your own if you like, by writing a 9kB
file to track 1 of the disk; this is loaded at 0xDC00 on warm boot.


Technical details
-----------------

The machine has a Z180 with 512MB of address space. The RAM is at 0x60000 to
0x6ffff, physical. cpmish maps the Brother OS out of memory and turns
interrupts off, using bare metal access to the hardware.

The terminal emulator is ADM-3a with some Kaypro II extensions, using a
custom (drawn by me!) 6x7 font. This appears to be a relatively common choice
and most software I've tried (Turbo Pascal, VDE etc) works. Sadly, games tend
not to work due to the non-standard screen size.


Who?
----

Everything here was written by me, David Given, and is covered under the
terms of the whole CP/Mish project. See the documentation in the project root
for more information.

