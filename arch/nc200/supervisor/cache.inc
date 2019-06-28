; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

; We keep five 9kB track buffers in the CACHE1/CACHE2/CACHE3 banks, used as an
; LRU disk cache.

CACHE_DEBUG:	equ 0
BUFFER_COUNT:	equ 5
RETRY_COUNT:	equ 10

; Each buffer has a descriptor containing these values:

B_BANK:		equ 0 ; bank that the bottom of this buffer lives in
B_ADDRESS:	equ 1 ; high byte of addess of the base of this buffer
B_CONTENTS:	equ 2 ; track that this buffer holds, or -1 if empty
B_LASTUSED:	equ 3 ; decrementing counter used for L1U cache
B_FLAGS:	equ 4 ; generic flags byte
B__SIZE:	equ 5

FLAG_DIRTY:	equ 0

; Flush unwritten changes to the disk.

cache_flush_unwritten:
		push ix
		ld b, BUFFER_COUNT
		ld ix, buffer_descriptors
.1:
		bit FLAG_DIRTY, (ix+B_FLAGS)
		jr z, .2
		push bc
		call flush_buffer
		pop bc
.2:
		ld de, B__SIZE
		add ix, de
		djnz .1
		pop ix
		ret

; Evict all buffers.

cache_evict_all:
		push ix
		ld b, BUFFER_COUNT
		ld ix, buffer_descriptors
.1:
		push bc
		call evict_buffer
		pop bc
		ld de, B__SIZE
		add ix, de
		djnz .1
		pop ix
		ret

; Reads/writes a CP/M 128-byte sector from the cache, reading in a track if
; necessary.
;
; On entry:
;   D = zero-based track to load
;   E = zero-based sector to load
;   HL = userspace address for data
;
; Returns nz on success, z on error.

cache_read128:
		call setup_read_or_write
		ret z
		push ix
		push iy

		call find_or_allocate_buffer
		jr z, .1
		call map_single_sector
		ld (buffer_address), hl

		ld hl, (current_dma)
		call map_userspace_dma

		ex de, hl               ; DE = destination address
		ld hl, (buffer_address) ; HL = address of sector
		ld bc, 128
		ldir

		or 1                    ; set nz
.1:
		pop iy
		pop ix
		ret

cache_write128:
		call setup_read_or_write
		ret z
		push ix
		push iy

		call find_or_allocate_buffer
		call map_single_sector
		ld (buffer_address), hl

		ld hl, (current_dma)
		call map_userspace_dma  ; HL = source address

		ld de, (buffer_address) ; DE = address of sector in buffer
		ld bc, 128
		ldir

		set FLAG_DIRTY, (ix+B_FLAGS) ; mark buffer as dirty

		pop iy
		pop ix

		or 1                    ; set nz
		ret

setup_read_or_write:
		ld a, d
		cp 80
		jr nc, out_of_bounds
		ld (current_track), a

		ld a, e
		cp 18*4
		jr nc, out_of_bounds
		ld (current_sector), a

		ld (current_dma), hl
		or 1                    ; set nz
		ret

out_of_bounds:
		cp a                    ; set z
		ret

; Finds the buffer for the current track, returning the buffer descriptor in
; IX. If there is no buffer for this track, allocate a new one and populate it
; from disk.
; Returns nz on success, z on failure.

find_or_allocate_buffer:
		; Try to find a buffer containing this track.

		ld a, (current_track)
		ld b, BUFFER_COUNT
		ld ix, buffer_descriptors
		ld de, 5
.1:
		cp (ix+B_CONTENTS)
		jr z, update_buffer_lru
		add ix, de
		djnz .1

		; No buffer was found. Search for the lowest B_LASTUSED.

		ld a, 0xff
		ld b, BUFFER_COUNT
		ld ix, buffer_descriptors
.3:
		cp (ix+B_LASTUSED)
		jr c, .2
		ld a, (ix+B_LASTUSED)
.2:
		add ix, de
		djnz .3

		; Now go through the loop again finding one with that B_LASTUSED.

		ld b, BUFFER_COUNT
		ld ix, buffer_descriptors
.4:
		cp (ix+B_LASTUSED)
		jr z, .5
		add ix, de
		djnz .4

		ld a, '!'
		call tty_putc
		jr $                        ; should never happen, panic
.5:

		; If the buffer has contents, evict it.

		ld a, (ix+B_CONTENTS)
		or a
		call z, evict_buffer

		; This is now our buffer. Load it.

		if CACHE_DEBUG
			ld a, '['
			call tty_putc
			ld a, 'R'
			call tty_putc
			ld a, (current_track)
			call tty_puthex8
			ld a, ']'
			call tty_putc
		endif

		ld a, (current_track)
		ld (ix+B_CONTENTS), a

		call map_entire_buffer
		ld a, RETRY_COUNT
		ld (retries), a
		ld a, (current_track)
		ld b, a
.6:
		push hl
		push bc
		call fd765_read_complete_track
		pop bc
		pop hl
		jr nz, update_buffer_lru    ; success
		push hl
		push bc
		call fd765_recalibrate_twice
		pop bc
		pop hl
		ld a, (retries)
		dec a
		ret z
		ld (retries), a
		jr .6

		; The buffer is loaded and stuff; A contains the buffer number.
update_buffer_lru:
		; Once more through the loop, decrementing all the LRU counters.

		ld b, BUFFER_COUNT
		ld iy, buffer_descriptors
		ld de, 5
.1:
		ld a, (iy+B_LASTUSED)
		or a
		jr z, .2
		dec a
		ld (iy+B_LASTUSED), a
.2:
		add iy, de
		djnz .1

		; ...except this buffer, which gets its counter reset.

		ld a, 0xff
		ld (ix+B_LASTUSED), a

		or 1                        ; set nz
		ret

; Flushes the current buffer and then discards the data in it.

evict_buffer:
		call flush_buffer
		ld a, 0xff
		ld (ix+B_CONTENTS), a
		ret

; Flushes the current buffer, marks it as clean, but keeps it.

flush_buffer:
		bit 7, (ix+B_CONTENTS)
		ret nz                  ; buffer is empty

		if CACHE_DEBUG
			ld a, '['
			call tty_putc
			ld a, 'E'
			call tty_putc
			ld a, (ix+B_CONTENTS)
			call tty_puthex8
			ld a, ']'
			call tty_putc
		endif

		bit FLAG_DIRTY, (ix+B_FLAGS)
		ret z                   ; buffer not dirty, don't write

		if CACHE_DEBUG
			ld a, '['
			call tty_putc
			ld a, 'W'
			call tty_putc
			ld a, (ix+B_CONTENTS)
			call tty_puthex8
			ld a, ']'
			call tty_putc
		endif

		call map_entire_buffer
		ld b, (ix+B_CONTENTS)
		call fd765_write_complete_track
		res FLAG_DIRTY, (ix+B_FLAGS)
		ret

; Maps buffer descriptor IX in the 0x4000..0xc000 range; returns the address
; of the buffer in HL.
map_entire_buffer:
		ld a, (ix+B_BANK)
		out (PORT_BANK1), a
		inc a
		out (PORT_BANK2), a

		ld l, 0
		ld h, (ix+B_ADDRESS)
		ret

; Maps the current CP/M sector in buffer descriptor IX in the 0x4000..0x7fff
; range; returns the address of the sector in HL.
map_single_sector:
		ld a, (current_sector)
		ld b, a
		ld c, 0
		srl b
		rr c                    ; bc = current_sector * 128

		ld a, (ix+B_BANK)
		out (PORT_BANK1), a
		ld h, (ix+B_ADDRESS)
		ld l, 0
		add hl, bc              ; calculate the address of the sector
		
		bit 7, h
		ret z

		; The sector's actually outside the 0x4000..0x7fff range.
		inc a
		out (PORT_BANK1), a
		ld a, h
		sub 0x40
		ld h, a
		ret
		
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

emit_buffer_descriptor macro n
		local addr, bank, offset
			addr = n * 0x2400
			bank = addr / 0x4000
			offset = 0x4000 + addr - (bank * 0x4000)
			db CACHE1_BANK + bank   ; B_BANK
			db offset / 0x100       ; B_ADDRESS
			db 0xff                 ; B_CONTENTS
			db 0                    ; B_LASTUSED
			db 0                    ; B_FLAGS
		endm

buffer_descriptors:
	count = 0
	rept BUFFER_COUNT
		emit_buffer_descriptor count
		count = count + 1
	endm

current_sector: db 0
current_track:  db 0
buffer_address: dw 0
current_dma:    dw 0
retries:        db 10
    
