#include "base.h"

int main(int argc, char* argv[]){
    forward_OPT = argv[1][0];
    predict_OPT = argv[2][0];
    start();
    // PC값이 0xffff:ffff될때까지 Simulator 구동
    // 단 PC값을 마지막에 갱신하고 체크해야 하므로, do-while 사용에 EX()를 마지막에 둔다.
    do{
        cycle++;
        /* Sequential Execution */
        MEM();
        WB();
        IF();
        ID();
        EX();
        /* Show Cycle State */
        print_cycle();
        /* Update latches */
        LAT_update();
    } while (PC != 0xffffffff);
    print_result();
    return 0;
}

void start(){
    printf("\n============================================================\n");
    printf("=================  Piplined MIPS Emulator  =================\n");
    printf("============================================================\n");
    cycle = 0;
    R_cnt = 0;
    I_cnt = 0;
    J_cnt = 0;
    brc_cnt = 0;
    success = 0;
    fail = 0;
    mem_cnt = 0;
    PC = 0x00000000;
    IM_init();
    RF_init();
    DM_init();
    CU_init();
    LAT_init();
    HU_init();
    FW_init();
}

void IF(){
    if (PC != 0xffffffff){
        IFID[0].valid = true;
        if (PCwrite == false) PC = IFID[1].nextPC;
        IR = IM_fetch(PC);
        set_val_IFID();
        // 다음 명령어 Fetch를 위해 branch prediction을 진행
        switch(predict_OPT){
            case '0':
                branch_DW();
                break;
            case '1':
                ANT_prediction();
                break;
            case '2':
                ALT_prediction();
                break;
            case '3':
            case '4':
            default:
                printf("Predictior Option Error.\n");
                break;
        }
    }
}

void ID(){
    if (IFID[1].valid == true){
        // Execute를 위한 Control Signal 준비, 값 설정 및 다음 latch로 전송
        CU_setting(IFID[1].opcode, IFID[1].funct);
        RF_read(IFID[1].rs, IFID[1].rt);
        IDEX[0].valid = true;
        set_sig_EXMEM();
        set_val_IDEX();
    }
}

void EX(){
    if (IDEX[1].valid == true){
        uint32_t ALUControl = ALUCU_control(IDEX[1].sig.ALUOp, MUX(IDEX[1].sig.RegDst, IDEX[1].opcode, IDEX[1].funct));
        EXMEM[0].ReadData1 = MUX3(fwA, IDEX[1].ReadData1, MUX(MEMWB[1].sig.MemtoReg, MEMWB[1].ALUResult, MEMWB[1].ReadData), EXMEM[1].ALUResult);
        EXMEM[0].ReadData2 = MUX3(fwB, IDEX[1].ReadData2, MUX(MEMWB[1].sig.MemtoReg, MEMWB[1].ALUResult, MEMWB[1].ReadData), EXMEM[1].ALUResult);
        EXMEM[0].ALUResult = ALU_calculate(ALUControl, MUX(IDEX[1].sig.Shift, EXMEM[0].ReadData1, EXMEM[0].ReadData2), MUX(IDEX[1].sig.Shift, MUX(IDEX[1].sig.ALUSrc, EXMEM[0].ReadData2, IDEX[1].imm), IDEX[1].shamt));
        EXMEM[0].valid = true;
        set_val_EXMEM();
        
        PCSrc = (IDEX[1].sig.Branch && !EXMEM[0].ALUResult) || (IDEX[1].sig.BranchNE && EXMEM[0].ALUResult);
        if (IDEX[1].sig.Branch || IDEX[1].sig.BranchNE){
            switch(predict_OPT){
            case '0':
                branch_operation();
                break;
            case '1':
                ANT_operation();
                break;
            case '2':
                ALT_operation();
                break;
            default:
                printf("Predictior Option Error.\n");
                break;
            }
        }
        
        // PC Update
        if (PCwrite){
            PC = MUX(IDEX[1].sig.Jump, MUX(IDEX[1].sig.JumpR, PC, EXMEM[0].ReadData1), jumpMaker(IDEX[1].nextPC, IDEX[1].address));
        }
        
    }
    set_sig_MEMWB();

    // Hazard Detection, Forwarding
    switch(forward_OPT){
        case '0':
            HU_operation_opt0();
            break;
        case '1':
            HU_operation_opt1();
            FW_operation();
            break;
        default:
            printf("Forwarding Option Error.\n");
            break;
    }
}

void MEM(){
    if (EXMEM[1].valid == true){
        DM_operation(EXMEM[1].sig.MemRead, EXMEM[1].sig.MemWrite, EXMEM[1].ALUResult, EXMEM[1].ReadData2, MUX3(EXMEM[1].sig.MemSize, 32, 16, 8));
        MEMWB[0].valid = true;
        set_val_MEMWB();
    }
}

void WB(){
    if (MEMWB[1].valid == true){
        uint32_t outData = MUX(MEMWB[1].sig.JumpAL, MUX(MEMWB[1].sig.MemtoReg, MEMWB[1].ALUResult, MEMWB[1].ReadData), MEMWB[1].nextPC + 4);
        DataSrc = (MEMWB[1].sig.MemSize == 0b00) || (MEMWB[1].sig.MemSize == 0b11);
        RF_write(MEMWB[1].sig.RegWrite, MEMWB[1].rd, MUX(DataSrc, MUX(MEMWB[1].sig.LoadU, signExtend(outData), zeroExtend(outData)), outData));
    }
}