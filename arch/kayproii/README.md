Platform: the Kaypro II
=======================

The [Kaypro II](http://oldcomputers.net/kayproii.html) is a stupendously
popular Z80-based machine originally coming out in 1982, with 64kB of RAM,
two 200kB drives, a character cell 80x25 screen and a rather slow 2.5MHz
processor.

Interestingly, it's got a big bank-switched ROM containing all the BIOS functionality, so the BIOS itself just delegates to this. This made porting CP/Mish to it very easy.

What you get:

  - everything: all the functionality of the original CP/M for the Kaypro II
    should work in CP/Mish. You can even use the same Kaypro configuration
    tools, or at least, I hope you can. (I haven't tried.)

How to use it
-------------

I've only ever used this on an emulator, but the disks should work on the
real hardware --- you'll need to get them onto the appropriate 5.25" disks.

To run CP/Mish in the [MESS](https://www.mamedev.org) emulator do this:

    mess kayproii -flop1 kayproii.img

It should Just Work (although remember you will need to install the Kaypro II
ROMs before the emulator will work).


Who?
----

Everything here was written by me, David Given, and is covered under the
terms of the whole CP/Mish project. See the documentation in the project root
for more information.

