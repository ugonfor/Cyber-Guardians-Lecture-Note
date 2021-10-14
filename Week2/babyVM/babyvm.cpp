#include "babyvm.h"

uint32_t reg1;
uint32_t reg2;
uint32_t pc;
uint32_t sp;

uint8_t ZF;
uint32_t stack[0x1000];

uint32_t add(uint32_t a1, uint32_t a2){ //1
    return a1 + a2;
}

uint32_t sub(uint32_t a1, uint32_t a2){ //2
    return a1 - a2;
}

void cmp(uint32_t a1, uint32_t a2){ //3
    if(a1 == a2) ZF = 1;
    else ZF = 0;
}

void push(uint32_t a1){ //4
    stack[sp] = a1;
    sp++;
}

uint32_t pop(void){ //5
    uint32_t tmp = stack[sp];
    sp--;
    return tmp;
}

void _printf(void){ //0
    std::cout << "REG INFO" << "\n";
    printf("reg1 : 0x%x\n", reg1);
    printf("reg2 : 0x%x\n", reg2);
    printf("pc   : 0x%x\n", pc);
    printf("sp   : 0x%x\n", sp);
    printf("ZF   : %d\n", ZF);
}

void _putc(void){ //9
    printf("%c%c%c%c\n",    (reg1>>24)  & 0xff,
                            (reg1>>16)  & 0xff,
                            (reg1>>8)   & 0xff,
                            (reg1)      & 0xff );
}

int babyVM(char code[]){
    uint32_t tmp;
    while (pc <= length)
    {
        switch (code[pc])
        {
        case '0':
            _printf();
            pc++;
            break;
        
        case '1':
            reg1 = add(reg1, reg2);
            pc++;
            break;
        
        case '2':
            reg1 = sub(reg1, reg2);
            pc++;
            break;
        
        case '3':
            cmp(reg1, reg2);
            pc++;
            break;
        
        case '4':
            push(reg1);
            pc++;
            break;

        case '5':
            tmp = *(uint32_t*)(&code[++pc]);
            push(tmp);
            pc += 4;
            break;
        
        case '6':
            reg1 = pop();
            pc++;
            break;

        case '7':
            reg2 = pop();
            pc++;
            break;
        
        case '8':
            reg2 = reg1;
            pc++;
            break;
        
        case '9':
            _putc();
            pc++;
            break;
        
        default:
            pc++;
            break;
        }
    }

    return 0;    
}