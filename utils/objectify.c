#include <stdio.h>

int main(int argc, const char* argv[])
{
    int count = 0;

    for (;;)
    {
        int c = getchar();
        if (c == EOF)
            break;

        if (count == 0)
            printf("\n\tdb ");
        else
            printf(", ");
        printf("0x%02x", c);

        count = (count+1) & 7;
    }
    printf("\n");
    return 0;
}
