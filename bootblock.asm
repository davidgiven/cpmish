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

org 0
macro safeorg, addr    
    if $ > addr
        .error Code passed fixed boundary
    endif
    ds addr - $
endm

; Sector 0: the main FAT boot sector.

safeorg 0x003
    db "NCBOOT "

    ; BIOS parameter block
safeorg 0x00b
    dw 512  ; bytes per sector
    db 2    ; sectors per cluster
    dw 2    ; reserved sectors
    db 2    ; FAT count
    dw 0x70 ; number of root directory entries
    dw 18   ; filesystem size, in sectors, not including the first two
    db 0xf9 ; media byte
    dw 3    ; sectors per FAT
    dw 9    ; number of sectors per track
    dw 2    ; number of heads
    dw 0    ; number of hidden sectors

    ; Boot signature
safeorg 0x1fe
    db 0x55, 0xaa

; Sector 1: the magic numbers needed to make the disk bootable.

safeorg 0x20b
    dw 512  ; bytes per sector
    db 2    ; sectors per cluster
    dw 0    ; reserved sectors
    db 2    ; FAT count
    dw 0x70 ; number of root directory entries
    dw 18   ; filesystem size, in sectors, not including the first two
    db 0xf9 ; media byte
    dw 3    ; sectors per FAT
    dw 9    ; number of sectors per track
    dw 2    ; number of heads
    dw 0    ; number of hidden sectors

; The first FAT (the only one the NC200 looks at).

    safeorg 0x400
    db 0xf9, 0xff, 0xff
    db 0xff, 0x7f, 0xff ; One reserved cluster, one used cluster

; The root directory.

safeorg 0x1000
    db "AUTO    PRG"
    db 0x20    ; Archive bit
    db 0x00    ; Unused flags
    db 0x00    ; 10ms time unit creation time
    dw 0x0000  ; Creation time
    dw 0x0000  ; Creation date
    dw 0x0000  ; Last access date
    dw 0       ; High bytes of cluster number
    dw 0x0000  ; Last modified time
    dw 0x0000  ; Last modified date
    dw 2       ; Low bytes of cluster number
    dw 1024, 0 ; File size (one cluster)

; The 1kB of code here gets loaded at 0x4000 as part of the boot process.
; At this point we take over from the NC200 OS, disable interupts, reshuffle
; the things, and load the supervisor proper.

safeorg 0x1e00
bootblock:
    di
    ld a, 0x40      ; First 16kB of physical RAM
    out (0x10), a   ; -> first 16kB RAM bank
    ld a, 0x20      ; Video memory address to 8kB (of physical RAM)
    out (0x00), a
    jr bootblock
    ret

; The data area stops at 0x2400.
if $ >= 0x2400
    .error Code bigger than 9kB
endif
