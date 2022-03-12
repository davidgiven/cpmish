Platform: the Brother WP-1 (or similar) word processor
======================================================

The WP-1 is a 1985 word processing appliance: a simple Z180 computer with an
amber letterbox character screen and 3.5" floppy disk drive built in to a
daisywheel typewriter chassis. It's got 64kB of RAM and a 91x15 screen, with an
excellent keyboard attached.

It's not really supposed to boot from floppy but Brother did release a small
amount of software for it, including an almost useless BASIC interpreter which
was unable to load or save programs and a typing tutor. I've managed to reverse
engineer the executable format and have cpmish working on it.

Sadly, this series of typewriters doesn't use ordinary 720kB or 1440kB DOS
floppies --- anything but! It's a custom GCR encoding scheme (implemented _in
software_!) which PC floppy disk drive controllers won't touch. Luckily you can
write these disks with a [FluxEngine](http://cowlark.com/fluxengine), but
you'll need to build a $10-15 board to connect a PC drive to to do that. See
[the FluxEngine Brother
documentation](http://cowlark.com/fluxengine/doc/disk-brother.html) for more
information. (You'll need it.)

What you get with this port:

- about 109kB of storage on a 120kB GCR disk (I have to reserve three tracks
  for the Brother's proprietary filesystem to boot from)
- most of an ADM-3a / Kaypro II terminal emulator supporting 90x15 text
- a pitiful 36kB TPA
- a non-interrupt driven keyboard which drops keypresses if the machine's busy
- a blinking cursor
- bugs

What you don't get:

- disk writing (the Brother system call for writing sectors seems to randomly
  corrupt the disk)
- sysgen, format etc
- no bugs

As you may gather, it's all a bit fragile.


How to use it
-------------

Build cpmish.

Use FluxEngine to write this to a DD 3.5" floppy, using a command line like:

    fluxengine write brother120 -i brotherwp1.img

Insert the disk into the machine's drive, and then power on while holding
CODE+Q. The machine will boot from the disk.

**Big warning:** if you save any Brother files onto the disk, you'll
irrevocably corrupt the CP/M filesystem. Don't do this.

You can replace the CCP and BIOS with your own if you like, although you do
need to do this from Linux (because I haven't written the CP/M tools for this
yet). They should be written onto the disk at offset 0x000d00.


Technical details
-----------------

The machine has a Z180 with 512MB of address space. The RAM is at 0x20000 to
0x2ffff, physical. cpmish lives from 0x27000 to 0x2ffff so as not to step on
the Brother OS's toes, which uses 0x20000 to 0x26fff for workspace; we rely on
it to do disk handling, as both are complex to use. Hence the very small TPA.

It'd be theoretically possible to dispense with the Brother OS entirely, and
access all the hardware ourselves. This would require reverse engineering the
details of how the floppy 'controller' works; this is done almost entirely in
software, so we'd need to reproduce all the code for encoding/decoding flux
patterns on the disk, etc; we'd also need to duplicate Brother's frankly crazy
microstepping logic for finding tracks on the disk (the drives don't have
adequate alignment, and make up for it by moving the head a fraction of a track
until they get a clean read!).

The terminal emulator is ADM-3a with some Kaypro II extensions. Currently
display attributes aren't supported.


Who?
----

Everything here was written by me, David Given, and is covered under the
terms of the whole CP/Mish project. See the documentation in the project root
for more information.


