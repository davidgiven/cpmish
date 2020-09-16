#include <stdio.h>
#include <z80ex/z80ex.h>

int main(void)
{
	Z80EX_VERSION *ver;
	
	printf("z80ex version info:\n-------------------\n");
	
	ver = z80ex_get_version();
	
	printf("API revision: %d\nmajor version number: %d\nminor version number: %d\nrelease type: %s\n",
		ver->API_revision, ver->major, ver->minor, ver->release_type);
	
	printf("full version number: %s\n", ver->as_string);
	
	return(0);
}
