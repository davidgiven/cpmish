org 0
db 0

; Sector 0: the main FAT boot sector.

    org 0x003
    db "NCBOOT "

    ; BIOS parameter block
    org 0x00b
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
    org 0x1fe
    db 0x55, 0xaa

; Sector 1: the magic numbers needed to make the disk bootable.

    org 0x20b
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

; FATs.

fat_definition: macro
    db 0xf9, 0xff, 0xff
    db 0xff, 0x7f, 0xff ; One reserved cluster, one used cluster
endm

; The two FATs.

    org 0x400
    fat_definition

;    org 0xa00
;    fat_definition

; The root directory.

    org 0x1000
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

    org 0x1e00
    incbin "auto.img"

; The data area stops at 0x2400.
