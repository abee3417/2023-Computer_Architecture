/* Control Unit C Code */
#include "base.h"

void CU_init(){
    memset(&IDEX[0].sig, 0, sizeof(Signal_IDEX));
}

void CU_setting(uint8_t opcode, uint8_t funct){
    IDEX[0].sig.Jump = (opcode == J) || (opcode == JAL);
    IDEX[0].sig.JumpAL = (opcode == JAL) || ((opcode == 0) && (funct == JALR));
    IDEX[0].sig.JumpR = ((opcode == 0) && (funct == JR)) || ((opcode == 0) && (funct == JALR));
    IDEX[0].sig.Branch = (opcode == BEQ);
    IDEX[0].sig.BranchNE = (opcode == BNE);
    IDEX[0].sig.ZeroEx = (opcode == ANDI) || (opcode == ORI);
    IDEX[0].sig.Shift = (opcode == 0) && ((funct == SLL) || (funct == SRL));
    IDEX[0].sig.RegDst = (opcode == 0);
    IDEX[0].sig.RegWrite = (opcode != BEQ) && (opcode != BNE) && (opcode != J) && (opcode != SB) && (opcode != SH) && (opcode != SW) && (funct != JR);
    IDEX[0].sig.MemWrite = (opcode == SB) || (opcode == SH) || (opcode == SW);
    IDEX[0].sig.MemRead = (opcode == LB) || (opcode == LBU) || (opcode == LH) || (opcode == LHU) || (opcode == LW);
    IDEX[0].sig.MemtoReg = (opcode == LB) || (opcode == LBU) || (opcode == LH) || (opcode == LHU) || (opcode == LW);
    IDEX[0].sig.LoadU = (opcode == LBU) || (opcode == LHU);
    IDEX[0].sig.ALUSrc = (opcode != 0) && (opcode != BEQ) && (opcode != BNE);


    /* ALU Control Unit을 위한 ALUOp 신호 */
    // LUI를 제외한 load와 store 관련 명령어들 : 0b00
    // LUI는 다른 load, store와 다르게 상수와 더해주는 연산이 없으므로 I-type에서 따로처리
    if ((opcode == LB) || (opcode == LBU) || (opcode == LH) || (opcode == LHU) || (opcode == LW) ||
    (opcode == SB) || (opcode == SH) || (opcode == SW)){
        IDEX[0].sig.ALUOp = 0b00;
    }
    // Branch 관련 명령어들 : 0b01
    else if ((opcode == BEQ) || (opcode == BNE)){
        IDEX[0].sig.ALUOp = 0b01;
    }
    // R-Type 명령어들 : 0b10
    else if ((opcode == 0)){
        IDEX[0].sig.ALUOp = 0b10;
    }
    // 그 외 : 0b11
    else{
        IDEX[0].sig.ALUOp = 0b11;
    }
    
    /* Data Memory를 위한 MemSize 신호 */
    if ((opcode == LH) || (opcode == LHU) || (opcode == SH)){
        IDEX[0].sig.MemSize = 0b01;
    }
    else if ((opcode == LB) || (opcode == LBU) || (opcode == SB)){
        IDEX[0].sig.MemSize = 0b10;
    }
    else{
        IDEX[0].sig.MemSize = 0b00;
    }
}