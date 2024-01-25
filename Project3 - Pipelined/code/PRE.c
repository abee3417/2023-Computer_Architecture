#include "base.h"

void branch_DW(){
    PC = PCAdder(PC);
    IFID[0].nextPC = PC;
    if (CH_DW_counter > 0){
        CH_DW_counter--;
        PC -= 4;
        if (CH_DW_counter == 0){
            PC += 4;
        }
    }
    else if ((IFID[0].opcode == BEQ || IFID[0].opcode == BNE)){
        printf("\nBranch Detected");
        CH_DW_counter = 2;
        //PC -= 4;
    }
}

void branch_operation(){
    if (PCSrc) brc_cnt++;
    if (PCwrite){
        PC = MUX(PCSrc, PC, branchAdder(IDEX[1].nextPC, IDEX[1].imm));
    }
}

void ANT_prediction(){
    PC = PCAdder(PC);
    IFID[0].nextPC = PC;
}

void ANT_operation(){
    if (PCSrc){
        // Branch Taken -> Prediction Fail
        brc_cnt++;
        fail++;
        // Stall을 위한 valid 변경
        IFID[0].valid = false;
        IDEX[0].valid = false;
    }
    else{
        // Branch Not Taken -> Prediction Success
        success++;
    }
    // PC update
    if (PCwrite){
        PC = MUX(PCSrc, PC, branchAdder(IDEX[1].nextPC, IDEX[1].imm));
    }
}

void ALT_prediction(){
    uint32_t tmp_imm;
    PC = PCAdder(PC);
    if (IFID[0].imm & 0x8000){
        tmp_imm = 0xffff0000 | IFID[0].imm;
    }
    else{
        tmp_imm = 0x0000ffff & IFID[0].imm;
    }
    if ((IFID[0].opcode == BEQ || IFID[0].opcode == BNE)){
        ALT_backupPC = PC; // Hazard 발생 대비용 백업
        PC = branchAdder(PC, tmp_imm);
    }
    IFID[0].nextPC = PC;
}

void ALT_operation(){
    if (PCSrc){
        // Branch Taken -> Prediction Success
        success++;
    }
    else{
        // Branch Not Taken -> Prediction Fail
        brc_cnt++;
        fail++;
        // Stall을 위한 valid 변경
        IFID[0].valid = false;
        IDEX[0].valid = false;
        PC = ALT_backupPC; // 백업한 PC값 불러오기
    }
}