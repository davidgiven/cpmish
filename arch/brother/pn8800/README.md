Platform: the Brother Super PowerNote PN8800MDS (or similar) word processor
===========================================================================

The Super PowerNote is a 1996 word processing laptop: a simple Z180 computer
with an old-fashioned LCD and a 3.5" floppy disk drive built in to a laptop
chassis. It's got 128kB of RAM and an 80x25 supertwist LCD screen, with an
excellent keyboard attached.

It's not really supposed to boot from floppy but Brother did release a small
amount of software for it, including Turn About, a Reversi clone. The hardware
is a 6MHz Z180, meaning it runs CP/M really well.

Luckily, this machine uses standard 1440kB DOS floppies, unlike some of their
other models.  So, cpmish disks can be easily created on a PC drive.

What you get with this port:

- about 1400kB of storage on a 1440kB MFM disk (I have to reserve two tracks for
  a FAT filesystem and the CP/M system track to boot from)
- a massive single-track disk cache meaning disk accesses are really fast
- most of an ADM-3a / Kaypro II terminal emulator supporting 80x25 text
- a respectable 58kB TPA
- a non-interrupt driven keyboard which drops keypresses if the machine's busy
- bugs

What you don't get:

- repeat key
- sysgen, format etc
- no bugs

As you may gather, it's all a bit fragile.


How to use it
-------------

Build cpmish.

Use `dd` or your favourite disk imager to write the `brotherpowernote.img` file
to an HD 3.5" floppy. You're probably best off formatting this on a PC as the
way the disk system works means that you want 1:1 interleave.

Insert the disk into the machine's drive, power on, and select 'Disk Application'
from the main menu. Then execute `CPMISH.APL`. After a few seconds cpmish should
start.

**Big warning:** if you save any Brother files onto the disk, you'll
irrevocably corrupt the CP/M filesystem. Don't do this.


Technical details
-----------------

The machine has a Z180 with 512MB of address space. The RAM is at 0x60000 to
0x7ffff, physical. cpmish maps the Brother OS out of memory and turns interrupts
off, using bare metal access to the hardware. 0x70000 to 0x80000 is used for
CP/M and 0x60000 to 0x70000 is caches (currently only the 9kB disk cache).

The terminal emulator is ADM-3a with some Kaypro II extensions.  This appears to
be a relatively common choice and most software I've tried (Turbo Pascal, VDE
etc) works.


Who?
----

Everything here was written by me, David Given, and is covered under the
terms of the whole CP/Mish project. See the documentation in the project root
for more information.
