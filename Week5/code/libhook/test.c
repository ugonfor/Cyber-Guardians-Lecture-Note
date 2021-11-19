#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc != 2){
        printf("usage: %s <int>\n", argv[0]);
        return 1;
    }

    for (int i = 0; i < atoi(argv[1]); i++)
    {
        printf("int : %d\n",i);
    }
    
    return 0;
}
