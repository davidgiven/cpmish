/*
 * Z80Ex, ZILoG Z80 CPU emulator.
 *
 * by Pigmaker57 aka boo_boo [pigmaker57@kahoh57.info]
 *
 * contains some code from the FUSE project (http://fuse-emulator.sourceforge.net)
 * Released under GNU GPL v2
 *
 */
 
#include <stdlib.h>
#include <string.h>
#define __USE_ISOC99
#include <stdio.h>
#include <stdarg.h>
 
#define __Z80EX_SELF_INCLUDE
#include "z80ex_dasm.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

typedef struct {
	const char *mnemonic;
	int t_states;
	int t_states2;
} z80ex_opc_dasm;

#include "opcodes/opcodes_dasm.c"

static const char *formats[] = {
	"#%02X", /*bytes*/	
	"#%04X", /*words*/
	"%d" /*WORDS_DEC, BYTES_DEC*/
};

#define STMP_SIZE 100

LIB_EXPORT int z80ex_dasm(char *output, int output_size, unsigned flags, int *t_states, int *t_states2,
	z80ex_dasm_readbyte_cb readbyte_cb, Z80EX_WORD addr, void *user_data)
{
	Z80EX_BYTE opc=0, next=0, disp_u=0;
	Z80EX_SIGNED_BYTE disp;
	int have_disp=0;
	int out_len=0;
	int bytes=0;
	const char *bytes_format=formats[0];
	const char *words_format=formats[1];
	const z80ex_opc_dasm *dasm = NULL;
	static char stmp[STMP_SIZE];
	
	if(flags & WORDS_DEC) words_format = formats[2];
	if(flags & BYTES_DEC) bytes_format = formats[2];
	
	*output='\0';
	*t_states=0;
	*t_states2=0;
	
	opc = readbyte_cb(addr++,user_data);
	bytes++;

	switch(opc)
	{
		case 0xDD:
		case 0xFD:
			next = readbyte_cb(addr++,user_data);
			if((next | 0x20) == 0xFD || next == 0xED)
			{
				strncpy(output,"NOP*",output_size-1);
				*t_states=4;
				dasm=NULL;
			}
			else if(next == 0xCB)
			{
				disp_u = readbyte_cb(addr++,user_data);
				next = readbyte_cb(addr++,user_data);
				bytes+=3;
				
				dasm = (opc==0xDD)? &dasm_ddcb[next]: &dasm_fdcb[next];
				have_disp=1;
			}
			else
			{
				bytes++;
				dasm = (opc==0xDD)? &dasm_dd[next]: &dasm_fd[next];
				if(dasm->mnemonic == NULL) /*mirrored instructions*/
				{
					dasm = &dasm_base[next];
					*t_states=4;
					*t_states2=4;
				}
			}
			break;
			
		case 0xED:
			next = readbyte_cb(addr++,user_data);
			bytes++;
			dasm = &dasm_ed[next];
			if(dasm->mnemonic == NULL)
			{
				strncpy(output,"NOP*",output_size-1);
				*t_states=8;
				dasm=NULL;
			}
			break;
			
		case 0xCB:
			next = readbyte_cb(addr++,user_data);
			bytes++;
			dasm = &dasm_cb[next];
			break;
		
		default:
			dasm = &dasm_base[opc];
			break;
	}
	
	if(dasm!=NULL)
	{	
		const char *mpos;
		int arglen;
		Z80EX_BYTE hi,lo; 
		char *outpos=output;
		
		for(mpos=(dasm->mnemonic); *mpos && out_len < output_size; mpos++)
		{
			*stmp='\0';
			
			switch(*mpos)
			{
				case '@':
					lo=readbyte_cb(addr++,user_data);
					hi=readbyte_cb(addr++,user_data);
					bytes+=2;
				
					arglen=snprintf(stmp,STMP_SIZE,words_format,(int)(lo+hi*0x100));
				
					break;
				
				case '$':
				case '%':
					if(!have_disp) disp_u = readbyte_cb(addr++,user_data);
					bytes++;
					disp = (disp_u & 0x80)? -(((~disp_u) & 0x7f)+1): disp_u;				
					
					if(*mpos == '$')
						arglen=snprintf(stmp,STMP_SIZE,bytes_format,(int)disp);
					else
						arglen=snprintf(stmp,STMP_SIZE,words_format,(int)((Z80EX_WORD)(addr+disp)));

					break;
				
				case '#':
					lo = readbyte_cb(addr++,user_data);
					bytes++;
				
					arglen=snprintf(stmp,STMP_SIZE,bytes_format,(int)lo);
				
					break;
				
				default:
					*(outpos++) = *mpos;
					out_len++;
					arglen=0;
					break;
			}
			
			if(arglen)
			{
				if(out_len+arglen >= output_size) break;
				strcpy(outpos,stmp);
				out_len+=arglen;
				outpos+=arglen;
			}
		}
		
		*outpos = '\0';
		
		*t_states+=dasm->t_states;
		*t_states2+=dasm->t_states2;
	}
	
	if(*t_states == *t_states2) *t_states2=0;
	
	return(bytes);
}


