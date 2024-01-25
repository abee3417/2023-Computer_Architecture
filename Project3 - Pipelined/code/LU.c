#include "base.h"

uint32_t signExtend(uint16_t imm){
    if (imm & 0x8000){
        return (0xffff0000 | imm);
    }
    else if ((imm & 0x0080) && !(imm & 0xff00)){
        return(0xffffff00 | imm);
    }
    else{
        if (!(imm & 0xff00)){
            return(0x000000ff & imm);
        }
        return (0x0000ffff & imm);
    }
}

uint32_t zeroExtend(uint16_t imm){
    if (!(imm & 0xff00)){
        return(0x000000ff & imm);
    }
    return (0x0000ffff & imm);
}

uint32_t PCAdder(uint32_t PC){
    return PC + 4;
}

uint32_t branchAdder(uint32_t PC, uint32_t imm){
    return PC + (imm << 2);
}

uint32_t jumpMaker(uint32_t PC, uint32_t address){
    return (PC & 0xf0000000) | (address << 2);
}
uint32_t MUX(bool sign, uint32_t input1, uint32_t input2){
    if (sign == 0){
        return input1;
    }
    else{
        return input2;
    }
}
uint32_t MUX3(uint8_t sign, uint32_t input1, uint32_t input2, uint32_t input3){
    switch(sign){
        case 0b00:
            return input1;
        case 0b01:
            return input2;
        case 0b10:
            return input3; 
    }
}