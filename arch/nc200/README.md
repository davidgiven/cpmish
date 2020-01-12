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
they're writeable from ordinary PC drives)
- support for a hard drive of up to 32MB on a Compact Flash card (note: not
SRAM)
- most of an ADM-3a / Kaypro II terminal emulator supporting 80x18 text
- a gigantic 60kB TPA
- an interrupt-driven keyboard
- parallel printer support
- bugs

What you don't get:

- power management (if you turn the machine off, all state is lost)
- repeat key
- blinking cursor
- serial port support
- sysgen, format etc
- no bugs


How to use it
-------------

Format a 720kB floppy disk on the NC200 and `dd` the `nc200.img` file onto it.

**Important:** the NC200 uses a non-standard sector skew to improve
performance. If you format your disks on a PC, they'll work, but using disks
formatted on the NC200 itself (using the standard ROM software) will more
than double disk speed.

Insert the disk into the machine's drive, power on, and press Code+R. CP/Mish
will boot.

**Big warning:** CP/Mish uses _all_ the memory in your system; if you have
files stored in RAM, **you will lose them**. The entire state of the machine
is destroyed. You have been warned!

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


Using a hard drive
------------------

If you want to store files on a hard drive, you'll need a PCMCIA type II ATA
card --- a Compact Flash card plugged into an inexpensive adaptor will do
just fine.

You will have trouble plugging this in to your NC200, because the slot in the
case is only big enough for a type I card, and type II cards are thicker. The
NC200 comes apart relatively easily and this can be removed, although
plugging a card in takes care without the plastic guide.

**Disclaimer:** Your NC200 was designed for type I cards only. While a type II
card works fine on mine, it may not work fine on yours --- there's more power
draw for a start, which may cause problems and potentially damage. Don't do
this unless you're willing to live with the consequences. If you damage your
machine, be aware that I will express sympathy but not compensate you in any
way: you have been warned!

The drive shows up as B: in CP/M. For simplicity no partition table is used
so the CP/M drive will occupy the entire card; however, the maximum
filesystem size is 32MB, so if you have a larger card (which seems likely)
any additional space will be wasted.

The card needs to be formatted before use. Use `mkfs b:` to do this. This
will destroy any data on your card.

Sadly, the NC200 is unable to boot from an ATA card, so you'll still need a
floppy disk in the drive to start cpmish; but once booted, your files will be
available on drive B.

If you have a CF card reader for a modern machine, the diskdefs file in the
cpmish root should allow access to files on the card:

    cpmcp -f nc200cf -i /dev/sdf 0:oncpm onlinux

Be aware that it's set up for a 32MB file system so if you have a smaller card
you will need to be careful writing to it.


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
contains the Supervisor, this leaves three banks spare; these are currently
unused.

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
