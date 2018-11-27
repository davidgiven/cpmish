#include <stdio.h>

int main(int argc, const char* argv)
{
    for (;;)
    {
        int c = getchar();
        if (c == EOF)
            break;
        printf("\t.db 0x%02x\n", c);
    }
    return 0;
}
