; The NC200 boots by looking at a warped FAT file system on the disk (mutated
; in very specific ways; see;
; http://cowlark.com/2017-12-04-nc200-reverse-engineering). If it finds an
; AUTO.PRG file on it, it'll load it at 0x4000 and run it.
;
; However, because the file system image is mostly empty, we abuse it by
; packing our supervisor into the cracks. So, once the boot block is loaded,
; we then read the entire cylinder (both heads) into memory and that's our
; supervisor. That will then cold-start CP/M by loading all of cylinder 1
; into memory.

include "include/supervisor.inc"
include "include/cpm.inc"
include "include/nc200.inc"

.text
.macro safeorg addr    
    .org \addr
.endm

; The main startup vector.

safeorg 0x000
    jp startup

; Sector 0: the main FAT boot sector.

safeorg 0x003
    .db "NCBOOT "

    ; BIOS parameter block
safeorg 0x00b
    .dw 512  ; bytes per sector
    .db 2    ; sectors per cluster
    .dw 2    ; reserved sectors
    .db 2    ; FAT count
    .dw 0x70 ; number of root directory entries
    .dw 18   ; filesystem size, in sectors, not including the first two
    .db 0xf9 ; media byte
    .dw 3    ; sectors per FAT
    .dw 9    ; number of sectors per track
    .dw 2    ; number of heads
    .dw 0    ; number of hidden sectors

.data.keyboard_bitmap:
    .ds 10, 0x55
.data.keyboard_buffer: ; must be contained within a single page
    .ds 8, 0x55
.data.keyboard_rdptr:
    .db 0
.data.keyboard_wrptr:
    .db 0
.data.approximate_time:
    .dw 0

; Interrupt vector
safeorg 0x038
    include "supervisor/interrupts.inc"
    include "supervisor/keyboard.inc"

    ; Boot signature
safeorg 0x1fe
    .db 0x55, 0xaa

; Sector 1: the magic numbers needed to make the disk bootable.

safeorg 0x20b
    .dw 512  ; bytes per sector
    .db 2    ; sectors per cluster
    .dw 0    ; reserved sectors
    .db 2    ; FAT count
    .dw 0x70 ; number of root directory entries
    .dw 18   ; filesystem size, in sectors, not including the first two
    .db 0xf9 ; media byte
    .dw 3    ; sectors per FAT
    .dw 9    ; number of sectors per track
    .dw 2    ; number of heads
    .dw 0    ; number of hidden sectors

include "supervisor/tty.inc"
include "supervisor/syscalls.inc"

; The first FAT (the only one the NC200 looks at).

    safeorg 0x400
    .db 0xf9, 0xff, 0xff
    .db 0xff, 0x7f, 0xff ; One reserved cluster, one used cluster

; The font.

.data.font:
    include "font.inc"

include "supervisor/startup.inc"
include "supervisor/floppy.inc"

; The root directory.

safeorg 0x1000
    .db "AUTO    PRG"
    .db 0x20    ; Archive bit
    .db 0x00    ; Unused flags
    .db 0x00    ; 10ms time unit creation time
    .dw 0x0000  ; Creation time
    .dw 0x0000  ; Creation date
    .dw 0x0000  ; Last access date
    .dw 0       ; High bytes of cluster number
    .dw 0x0000  ; Last modified time
    .dw 0x0000  ; Last modified date
    .dw 2       ; Low bytes of cluster number
    .dw 1024, 0 ; File size (one cluster)

; The 1kB of code here gets loaded at 0x4000 as part of the boot process.
; At this point we take over from the NC200 OS, disable interupts, reshuffle
; the things, and load the supervisor proper.
;
; On entry, the OS puts bank 0x40 at 0x0000 and a different bank (we don't
; care which one) at 0x4000. This means we can write to bank 0x40 without
; stepping on our own toes.

safeorg 0x1e00
bootblock:
    incbin "supervisor/auto.img"
