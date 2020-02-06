Platform: the Brother LW-30 (or similar) word processor
=======================================================

The LW-30 is a 1985 word processing appliance: a simple Z180 computer with an
LCD screen and 3.5" floppy disk drive built in to a daisywheel typewriter
chassis. The LW-30 appears to be the French/German version of the WP-1400D.
It's got 64kB of RAM and a 80x14 screen, with an excellent keyboard attached.

It's not really supposed to boot from floppy but Brother did release a small
amount of software for it, including
[Tetris](https://youtube.com/watch?v=3lSHfCdPRgw). I've managed to reverse
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

- about 230kB of storage on a 240kB GCR disk (I have to reserve four tracks for
  a FAT filesystem to boot from)
- most of an ADM-3a / Kaypro II terminal emulator supporting 80x14 test
- a pitiful 38kB TPA
- a non-interrupt driven keyboard which drops keypresses if the machine's busy
- a blinking cursor
- bugs

What you don't get:

- repeat key
- reliable disk writing (there are bugs here)
- support for the printer
- sysgen, format etc
- no bugs

As you may gather, it's all a bit fragile.


How to use it
-------------

Build cpmish.

Use FluxEngine to write this to a DD 3.5" floppy, using a command line like:

    fluxengine write brother -i brotherop2.img

Insert the disk into the machine's drive, power on, and press FILE to open the
disk menu.

**Big warning:** if you save any Brother files onto the disk, you'll
irrevocably corrupt the CP/M filesystem. Don't do this.

Press CODE+Q to open the programs menu, and then press ENTER to load and run
CPMISH.OP2. After a few seconds the screen will go blank, and after a few more
seconds cpmish should start.

To exit, press FILE and the machine will instantly restart (losing all your
work).

You can replace the CCP and BIOS with your own if you like, although you do
need to do this from Linux (because I haven't written the CP/M tools for this
yet).


Technical details
-----------------

The machine has a Z180 with 512MB of address space. The RAM is at 0x60000 to
0x6ffff, physical. cpmish lives from 0x65000 to 0x6ffff so as not to step on
the Brother OS's toes; we call out to it to do keyboard and disk handling, as
both are complex to use. Hence the very small TPA.

It'd be theoretically possible to dispense with the Brother OS entirely, and
access all the hardware ourselves. This would require reverse engineering the
details of how the floppy 'controller' works; this is done almost entirely in
software, so we'd need to reproduce all the code for encoding/decoding flux
patterns on the disk, etc; we'd also need to duplicate Brother's frankly crazy
microstepping logic for finding tracks on the disk (the drives don't have
adequate alignment, and make up for it by moving the head a fraction of a track
until they get a clean read!).

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

