#include "babyvm.h"

char buf[BUFSIZ];
uint32_t length;

uint32_t _input(char buf[]){
    uint32_t len = 0;
    char tmp;
    while (len < BUFSIZ-1)
    {
        scanf("%c", &tmp);
        
        if(tmp == '\n') break;
        buf[len] = tmp;
        len++;
    }
    return len;
}

int main(int argc, char const *argv[])
{
    std::cout << "[START] babyVM\n";

    printf("code : ");
    length = _input(buf);
    printf("len : %u\n", length);

    babyVM(buf);

    printf("[END] babyVM\n");
    return 0;
}
