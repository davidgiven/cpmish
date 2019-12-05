; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.
		
; Maps 128 consecutive bytes in userspace HL in the 0x8000..0xffff range;
; returns the address in HL.
map_userspace_dma:
	ld a, h
	and 0xc0                ; just the top two bits
	rlca
	rlca                    ; to the bottom two bits
	add a, USER0_BANK       ; calculate bank

	out (PORT_BANK2), a
	inc a
	out (PORT_BANK3), a

	ld a, h
	and 0x3f                ; bottom six bits
	add a, 0x80             ; adjust base
	ld h, a

	ret

; Given IX pointing to a deblocker structure, reads a sector.
generic_read128:
    ld a, 0
    call DB_GET
    ret z

    ; HL now points to the loaded buffer. Copy into userspace.

    push hl
    ld hl, (current_dma)
    call map_userspace_dma
    ex de, hl               ; DE = destination address
    pop hl                  ; HL = source address
    ld bc, 128
    ldir
    
    or 1                    ; set nz (success)
    ret

; Given IY pointing to a deblocker structure, writes a sector.
generic_write128:
    ld a, (deblock_flag)
    call DB_GET
    ret z

    ; HL now points to the loaded buffer. Copy from userspace.

    push hl
    ld hl, (current_dma)
    call map_userspace_dma  ; HL = source address
    pop de                  ; DE = destination address
    ld bc, 128
    ldir
    ld (ix+DBS_DIRTY), 1    ; mark block dirty

    ld a, (deblock_flag)
    cp 1
    jr nz, .1               ; no need to write back

    jp DB_FLUSH
.1
    or 1
    ret

