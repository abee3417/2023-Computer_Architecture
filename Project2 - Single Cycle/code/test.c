#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void print(uint16_t a){
    printf("Before : %d\n", a);
    printf("test : %x\n", !(a & 0x8000));
    if (a & 0x8000){
        uint32_t b = 0xffff0000 | a;
        printf("%d\n", b);
    }
    else{
        uint32_t b = 0x0000ffff & a;
        printf("%d\n", b);
    }
}
int main(){
    uint8_t a;
    uint32_t test = 0x0000ffff;

    bool brc=0;
    bool brcne=1;
    uint32_t result=0;
    uint32_t resultne=0xffff;
    printf("%d\n", resultne);
    printf("%d\n", !resultne);
    bool flag = (brc && !result) || (brcne && resultne);
    //printf("%x\n", test);
    //print(test);
    printf("%d\n", flag);

    return 0;
}