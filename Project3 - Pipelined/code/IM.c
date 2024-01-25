/* Instruction Memory C Code */
#include "base.h"

void IM_init(){
    FILE* f = fopen("input1.bin", "r");
    uint8_t buffer; // 파일입출력용 버퍼
    int site = 0;
    int flag = 0;
    if (f == NULL){ // 예외처리 : 파일이 없을 경우
        printf("FILE ERROR\n");
        return;
    }
    // Big Endian 구조
    while(fread(&buffer, sizeof(uint8_t), 1, f) != 0){
        InsMemory[site + flag] = buffer & 0xff;
        flag++;
        if (flag > 3){
            flag = 0;
            site += 4;
        }       
    }
    fclose(f);
}

uint32_t IM_fetch(uint32_t address){
    return (InsMemory[address] << 24) | (InsMemory[address + 1] << 16) |
            (InsMemory[address + 2] << 8) | (InsMemory[address + 3]);
}

/* 
컴파일 방법 (교수님 방법)
1. 컴파일 후 obj파일 생성
mips-linux-gnu-gcc -c 이름.c -mips1 -mfp32
2. obj파일로 bin파일 생성
mips-linux-gnu-objcopy -O binary -j .text 이름.o 이름.bin
3. obj파일을 mips로 보고싶을 때
mips-linux-gnu-objdump -d 이름.o
*/