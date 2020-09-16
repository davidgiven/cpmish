#include <stdlib.h>
#include <stdio.h>
#include <z80ex/z80ex_dasm.h>

Z80EX_BYTE *data=NULL;
int data_len=0;

void usage(char *name)
{
	printf("simple z80 code disassembler\nusage: %s binary_file [base_adress]\n",name);
}

Z80EX_BYTE readbyte_cb(Z80EX_WORD addr, void *user_data)
{
	int base_addr = *((int *)user_data);	
	return(data[addr-base_addr]);
}

int main(int argc,char *argv[])
{
	FILE *fp;
	int base_addr=0,addr=0;
	int t,t2;
	char buf[80];
	
	if(argc < 2 || argc > 3)
	{
		usage(argv[0]);
		return(0);
	}
	
	fp=fopen(argv[1],"rb");
	if(fp==NULL) goto fail;
		
	fseek(fp,0,SEEK_END);
	data_len=ftell(fp);
	fseek(fp,0,SEEK_SET);
	
	if(NULL==(data=(Z80EX_BYTE *)malloc(data_len))) goto fail;
	if(fread(data,1,data_len,fp)!=data_len) goto fail;
	fclose(fp);

	if(argc == 3) base_addr=atoi(argv[2]);

	addr=base_addr;
	while((addr-base_addr) < data_len)
	{
		printf("%04X: ",addr);
		addr+=z80ex_dasm(buf,80,0,&t,&t2,readbyte_cb,addr,&base_addr);
		printf("%-15s  t=%d",buf,t);
		if(t2) printf("/%d",t2);
		printf("\n");
	}
	
	free(data);
	return(0);
	
fail:
	fprintf(stderr,"ERROR\n");
	if(data!=NULL) free(data);
	return(1);
	
}
