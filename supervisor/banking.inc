; The NC200 has eight 16kB RAM banks.
;
; On entry to and exit from a system call, SUPERVISOR_BANK and USER3_BANK are
; mapped, but once the supervisor is running, memory from 0x4000 up can vary.
; We map stuff in as needed.

SUPERVISOR_BANK = 0x40 ; -> 0x0000
CACHE1_BANK     = 0x41
CACHE2_BANK     = 0x42
CACHE3_BANK     = 0x43
USER0_BANK      = 0x44 ; -> 0x0000
USER1_BANK      = 0x45 ; -> 0x4000
USER2_BANK      = 0x46 ; -> 0x8000
USER3_BANK      = 0x47 ; -> 0xc000
