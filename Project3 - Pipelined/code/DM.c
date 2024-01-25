/* Data Memory C Code */
#include "base.h"

void DM_init(){
    for (int i = 0; i < MAX_SIZE; i++){
        DataMemory[i] = 0;
    }
}

void DM_operation(bool MemRead, bool MemWrite, uint32_t address, uint32_t writeData, int size){
    if (MemRead == 1){ // LW일 경우
        mem_cnt++;
        uint32_t tmp = 0;
        for (int i = 0; i < (size / 8); i++){
            tmp = tmp << 8;
            tmp = tmp | DataMemory[address + i];
        }
        MEMWB[0].ReadData = tmp;
    }
    else if (MemWrite == 1){ // SW일 경우
        mem_cnt++;
        for (int i = (size / 8) - 1; i >= 0; i--){
            DataMemory[address + i] = writeData & 0xff; // 오른쪽 1바이트만 저장
            writeData = writeData >> 8; // right bit shift
        }
        
    }
}