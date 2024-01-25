/* Control Unit C Code */
#include "base.h"

void CU_init(){
    Jump = 0;
    JumpR = 0;
    JumpAL = 0;
    Branch = 0;
    BranchNE = 0;
    Shift = 0;
    RegDst = 0;
    RegWrite = 0;
    MemWrite = 0;
    MemRead = 0;
    MemtoReg = 0;
    ALUSrc = 0;
    PCSrc = 0;
    ALUOp = 0b00;
}

void CU_setting(uint8_t opcode, uint8_t funct){ // JR이라는 특수 케이스로 인하여 funct 필드도 인자로 받는다.
    Jump = (opcode == J) || (opcode == JAL);
    JumpR = (opcode == 0) && (funct == JR);
    JumpAL = (opcode == JAL);
    Branch = (opcode == BEQ);
    BranchNE = (opcode == BNE);
    Shift = (opcode == 0) && ((funct == SLL) || (funct == SRL));
    RegDst = (opcode == 0);
    RegWrite = (opcode != BEQ) && (opcode != BNE) && (opcode != J) && (opcode != SW) && (funct != JR);
    MemWrite = (opcode == SW);
    MemRead = (opcode == LW);
    MemtoReg = (opcode == LW);
    ALUSrc = (opcode != 0) && (opcode != BEQ) && (opcode != BNE);
    
    // LUI를 제외한 load와 store 관련 명령어들 : 0x00
    // LUI는 다른 load, store와 다르게 상수와 더해주는 연산이 없으므로 I-type에서 따로처리
    if ((opcode == LBU) || (opcode == LHU) || (opcode == LL) || (opcode == LW) ||
    (opcode == SB) || (opcode == SC) || (opcode == SH) || (opcode == SW)){
        ALUOp = 0b00;
    }
    // Branch 관련 명령어들 : 0x01
    else if ((opcode == BEQ) || (opcode == BNE)){
        ALUOp = 0b01;
    }
    // R-Type 명령어들 : 0x10
    else if ((opcode == 0)){
        ALUOp = 0b10;
    }
    // 그 외 : 0x11
    else{
        ALUOp = 0b11;
    }
}