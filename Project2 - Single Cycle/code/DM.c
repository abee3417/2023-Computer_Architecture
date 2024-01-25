/* Data Memory C Code */
#include "base.h"

void DM_init(){
    for (int i = 0; i < MAX_SIZE; i++){
        DataMemory[i] = 0;
    }
}

void DM_operation(bool MemRead, bool MemWrite, uint32_t address, uint32_t writeData){
    if (MemRead == 1){ // LW일 경우
        ReadData = DataMemory[address];
    }
    else if (MemWrite == 1){ // SW일 경우
        for (int i = 0; i < 4; i++){
            DataMemory[address + i] = writeData;
            writeData = writeData;
        }
        // DataMemory[address] = writeData;
    }
}