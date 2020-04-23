; System function equates

.EXIT	equ	0	; exit program
.EXIF	equ	1	; exit program if event flag not set
.EMTST	equ	2	; emit status
.QIO	equ	3	; queue I/O packet
.CONN	equ	4	; connect to task
.GTCMD	equ	5	; get command line arguments
.SEND	equ	6	; send data
.RECV	equ	7	; receive data
.WTDAT	equ	8	; wait for data
.ASTDF	equ	9	; declare AST service routine
.ASTCT	equ	10	; AST control (enable/disable AST types)
.ASTX	equ	11	; exit AST service routine
.ALUN	equ	12	; assign LUN to device
.GTLUN	equ	13	; get LUN information
.GDIR	equ	14	; get current dir name
.SDIR	equ	15	; set current dir
.GDAT	equ	16	; get date and time
.SDAT	equ	17	; set date and time
.RQST	equ	18	; request installed task
.RPOI	equ	19	; request and pass offspring information
.ABORT	equ	20	; abort a task
.STOP	equ	21	; stop task
.RESUM	equ	22	; resume task
.CLEF	equ	23	; clear event flag
.SETF	equ	24	; set single event flag
.RDEF	equ	25	; read event flag
.WTSE	equ	26	; wait for single event flag
.STSE	equ	27	; stop for single event flag
.WTLO	equ	28	; wait for logical OR of event flags
.STLO	equ	29	; stop for logical OR of event flags
.RUN	equ	30	; request installed task at specified time
.CSRQ	equ	31	; cancel scheduled task request
.MRKT	equ	32	; mark time request
.CMKT	equ	33	; cancel mark time request
.EXTSK	equ	34	; extend task size
.ALTPR	equ	35	; change task priority
.SUPER	equ	36	; enter/exit supervisor mode
.GTSK	equ	37	; get task info
.GTPAR	equ	38	; get partition info
.GIN	equ	39	; get general info

MAXFN	equ	39

SYSRST	equ	20h	; system call RST is RST 4
DBGRST	equ	30h	; debug breakpoint RST is RST 6

; .GDIR subfunction codes

GD.TSK	equ	0	; get task's current directory
GD.TI	equ	1	; get terminal's current directory
GD.LOG	equ	2	; get terminal's login directory

; .SDIR subfunction codes

SD.TSK	equ	0	; set task's current directory
SD.TI	equ	1	; set terminal's current directory

; .GIN subfunction codes

I.VERS	equ	0	; get system version
I.SMEM	equ	1	; get system memory info
I.USCB	equ	2	; get user's SCB
I.TSCB	equ	3	; get SCB of terminal device
I.HOST	equ	4	; get host name
I.TCKS	equ	5	; get ticks per second
I.FEAT	equ	6	; get feature bits

; Macro to call system services

SC	MACRO	func
	rst	SYSRST
	db	func
	ENDM

