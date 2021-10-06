#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main(int argc, char const *argv[])
{
    printf("%d\n", func1(atoi(argv[1]), atoi(argv[2])));
    return 0;
}
