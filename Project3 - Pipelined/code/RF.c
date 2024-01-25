/* Register File C Code */
#include "base.h"

void RF_init(){
    for (int i = 0; i < sp; i++){
        Register[i] = 0;
    }
    // r29 ~ r31 문제에 맞춰서 값 재설정
    Register[sp] = 0x1000000;
    Register[fp] = 0;
    Register[ra] = 0xffffffff;
}

void RF_read(uint8_t rs, uint8_t rt){
    IDEX[0].ReadData1 = Register[rs];
    IDEX[0].ReadData2 = Register[rt];
}

void RF_write(bool RegWrite, uint8_t write_r, uint32_t write_data){
    if (RegWrite == 1){
        Register[write_r] = write_data;
    }
}

