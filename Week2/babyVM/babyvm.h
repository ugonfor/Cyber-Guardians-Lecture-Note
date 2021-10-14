#pragma once

// c header
#include <stdio.h>
#include <stdint.h>

// cpp header
#include <iostream>
#include <string>

extern uint32_t reg1;
extern uint32_t reg2;
extern uint32_t pc;
extern uint32_t sp;

extern uint8_t ZF;
extern uint32_t stack[0x1000];

extern char buf[BUFSIZ];
extern uint32_t length;


uint32_t add(uint32_t a1, uint32_t a2);
uint32_t sub(uint32_t a1, uint32_t a2);
void cmp(uint32_t a1, uint32_t a2);

void push(uint32_t a1);
uint32_t pop(void);

void _printf(void);
void _putc(void);

int babyVM(char code[]);