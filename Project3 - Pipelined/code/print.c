#include "base.h"

void print_cycle(){
    printf("\nCycle[%d]\n", cycle);
    // IF 출력
    //printf("fwA : %d, fwB : %d\n", fwA, fwB);
    if (PCwrite == false){
        printf("[IF] PC: 0x%x, IR: 0x%08x\n", IFID[1].nextPC, IR);
    }
    else if (PC != 0xffffffff){
        printf("[IF] PC: 0x%x, IR: 0x%08x\n", IFID[1].nextPC - 4, IR);
    }
    else{
        printf("[IF] End of Cycle\n");
    }
    // ID 출력
    if (IFID[1].valid == true){
        char type;
        switch(IFID[1].opcode){
            case 0x00:
                type = 'R';
                break;
            case 0x02:
            case 0x03:
                type = 'J';
                break;
            default:
                type = 'I';
                break;
        }
        printf("[ID] type: %c, opcode: 0x%x, ",type, IFID[1].opcode);
        switch(type){
            case 'R':
                // nop은 counting 제외 
                if ((IFID[1].opcode | IFID[1].address) != 0x00000000){
                    if (IFIDwrite == true) R_cnt++;
                }
                printf("rs: 0x%x (r[%d]=0x%x), rt: 0x%x (r[%d]=0x%x), rd: 0x%x (r[%d]=0x%x), shamt: 0x%x, funct: 0x%x\n",
                        IFID[1].rs, IFID[1].rs, Register[IFID[1].rs], IFID[1].rt, IFID[1].rt, Register[IFID[1].rt], IFID[1].rd, IFID[1].rd, Register[IFID[1].rd], IFID[1].shamt, IFID[1].funct);
                break;
            case 'I':
                if (IFIDwrite == true) I_cnt++;
                printf("rs: 0x%x (r[%d]=0x%x), rt: 0x%x (r[%d]=0x%x), imm: 0x%x\n",
                        IFID[1].rs, IFID[1].rs, Register[IFID[1].rs], IFID[1].rt, IFID[1].rt, Register[IFID[1].rt], IFID[1].imm);
                break;
            case 'J':
                if (IFIDwrite == true) J_cnt++;
                printf("address: 0x%x\n", IFID[1].address);
                break;
            default:
                printf("Instruction Error.\n");
                break;
        }
    }
    else{
        printf("[ID] Stall\n");
    }
    // EX 출력
    if (IDEX[1].valid == true){
        switch(IDEX[1].opcode){
            // R type
            case 0x00:
                switch(IDEX[1].funct){
                    case ADD:
                    case ADDU:
                        printf("[EX] r[%d] <- 0x%x = r[%d] + r[%d]\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case AND:
                        printf("[EX] r[%d] <- 0x%x = r[%d] & r[%d]\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case JR:
                        printf("[EX] (Jump) PC <- 0x%x = r[%d]\n", PC, IDEX[1].rs);
                        break;
                    case JALR:
                        printf("[EX] r[31] <- 0x%x, (Jump) PC <- 0x%x = r[%d]\n", IDEX[1].nextPC + 4, PC, IDEX[1].rs);
                        break;
                    case NOR:
                        printf("[EX] r[%d] <- 0x%x = ~(r[%d] | r[%d])\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case OR:
                        printf("[EX] r[%d] <- 0x%x = r[%d] | r[%d]\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case SLT:
                    case SLTU:
                        printf("[EX] r[%d] <- %d = (r[%d] < r[%d]) ? 1 : 0\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case SLL:
                        printf("[EX] r[%d] <- 0x%x = r[%d] << %d\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case SRL:
                        printf("[EX] r[%d] <- 0x%x = r[%d] >> %d\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                    case SUB:
                    case SUBU:
                        printf("[EX] r[%d] <- 0x%x = r[%d] - r[%d]\n", IDEX[1].rd, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].rt);
                        break;
                }
                break;
            // I type
            case ADDI:
            case ADDIU:
                printf("[EX] r[%d] <- 0x%x = r[%d] + 0x%08x\n", IDEX[1].rt, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].imm);
                break;
            case ANDI:
                printf("[EX] r[%d] <- 0x%x = r[%d] & 0x%08x\n", IDEX[1].rt, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].imm);
                break;
            case BEQ:
                if (EXMEM[0].sig.Branch && !EXMEM[0].ALUResult){
                    if (predict_OPT == '2'){
                        printf("[EX] (Jump : Branch taken) PC <- 0x%x : R[%d] == R[%d]\n", IFID[1].nextPC-4, IDEX[1].rs, IDEX[1].rt);
                    }
                    else{
                        printf("[EX] (Jump : Branch taken) PC <- 0x%x : R[%d] == R[%d]\n", PC, IDEX[1].rs, IDEX[1].rt);
                    }
                }
                else{
                    printf("[EX] (Branch not taken) R[%d] != R[%d]\n", IDEX[1].rs, IDEX[1].rt);
                }
                break;
            case BNE:
                if(EXMEM[0].sig.BranchNE && EXMEM[0].ALUResult){
                    if (predict_OPT == '2'){
                        printf("[EX] (Jump : Branch taken) PC <- 0x%x : R[%d] == R[%d]\n", IFID[1].nextPC-4, IDEX[1].rs, IDEX[1].rt);
                    }
                    else{
                        printf("[EX] (Jump : Branch taken) PC <- 0x%x : R[%d] != R[%d]\n", PC, IDEX[1].rs, IDEX[1].rt);
                    }
                }
                else{
                    printf("[EX] (Branch not taken) R[%d] == R[%d]\n", IDEX[1].rs, IDEX[1].rt);
                }
                break;
            case LB:
            case LBU:
            case LH:
            case LHU:
            case LW:
                printf("[EX] Load Address : 0x%08x = r[%d] + 0x%08x\n", EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].imm);
                break;
            case LUI:
                printf("[EX] r[%d] <- 0x%x = 0x%x << 16\n", IDEX[1].rt, EXMEM[0].ALUResult, IDEX[1].imm);
                break;
            case ORI:
                printf("[EX] r[%d] <- 0x%x = r[%d] | 0x%08x\n", IDEX[1].rt, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].imm);
                break;
            case SLTI:
            case SLTIU:
                printf("[EX] r[%d] <- %d = (r[%d] < 0x%08x) ? 1 : 0\n", IDEX[1].rt, EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].imm);
                break;
            case SB:
            case SH:
            case SW:
                printf("[EX] Store Address : 0x%08x = r[%d] + 0x%08x\n", EXMEM[0].ALUResult, IDEX[1].rs, IDEX[1].imm);
                break;
            // J type
            case J:
                printf("[PC Update] (Jump) PC <- 0x%08x\n", PC);
                break;
            case JAL:
                printf("[EX] r[31] <- 0x%x, (Jump) PC <- 0x%x\n", IDEX[1].nextPC + 4, PC);
                break;
        }
    }
    else{
        printf("[EX] Stall\n");
    }
    // MEM 출력
    if (EXMEM[1].valid == true){
        if (EXMEM[1].sig.MemRead && !EXMEM[1].sig.MemWrite){
            printf("[MEM] Load Data : 0x%x = M[0x%08x]\n", MEMWB[0].ReadData, EXMEM[1].ALUResult);
        }
        else if (!EXMEM[1].sig.MemRead && EXMEM[1].sig.MemWrite){
            printf("[MEM] Store Data : M[0x%08x] = 0x%x\n", EXMEM[1].ALUResult, EXMEM[1].ReadData2);
        }
        else{
            printf("[MEM] No memory access\n");
        }
    }
    else{
        printf("[MEM] Stall\n");
    }
    // WB 출력
    if (MEMWB[1].valid == true){
        if (MEMWB[1].sig.RegWrite == true){
            uint32_t outData = MUX(MEMWB[1].sig.JumpAL, MUX(MEMWB[1].sig.MemtoReg, MEMWB[1].ALUResult, MEMWB[1].ReadData), MEMWB[1].nextPC);
            printf("[WB] R[%d] = 0x%x\n", MEMWB[1].rd, MUX(DataSrc, MUX(MEMWB[1].sig.LoadU, signExtend(outData), zeroExtend(outData)), outData));
        }
        else{
            printf("[WB] No write back\n");
        }
    }
    else{
        printf("[WB] Stall\n");
    }
}

void print_result(){
    printf("\n************************   Result   ************************\n\n");
    switch(forward_OPT){
        case '0':
            printf("Data Hazards : Detect and wait\n");
            break;
        case '1':
            printf("Data Hazards : Forwarding\n");
            break;
        default:
            printf("ERROR : No data hazards option\n");
            break;
    }
    switch(predict_OPT){
        case '0':
            printf("Control Hazards : Detect and wait\n");
            break;
        case '1':
            printf("Control Hazards : Static branch prediction - ANT\n");
            break;
        case '2':
            printf("Control Hazards : Static branch prediction - ALT\n");
            break;
        default:
            printf("ERROR : No control hazards option\n");
            break;
    }
    printf("Return Value (r2): %d (0x%x)\n", Register[v0], Register[v0]);
    printf("Total Cycle: %d\n", cycle);
    printf("Executed 'R' Instruction: %d\n", R_cnt);
    printf("Executed 'I' Instruction: %d\n", I_cnt);
    printf("Executed 'J' Instruction: %d\n", J_cnt);
    printf("Number of Branch Taken: %d\n", brc_cnt);
    printf("Number of Predict Success: %d\n", success);
    printf("Number of Predict Fail: %d\n", fail);
    printf("Number of Memory Access Instruction: %d\n", mem_cnt);
    printf("\n***************  End of Pipelined Emulator!  ***************\n\n");
}