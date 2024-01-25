#include "base.h"

void LAT_init(){
    // Latch valid를 일단 모두 false
    IFID[0].valid = false;
    IDEX[0].valid = false;
    EXMEM[0].valid = false;
    MEMWB[0].valid = false;

    // 레지스터 내의 요소들을 memset을 사용하여 모두 0으로 초기화
    memset(&IFID[0], 0, sizeof(Latch_IFID));
    memset(&IFID[1], 0, sizeof(Latch_IFID));
    memset(&IDEX[0], 0, sizeof(Latch_IDEX));
    memset(&IDEX[1], 0, sizeof(Latch_IDEX));
    memset(&EXMEM[0], 0, sizeof(Latch_EXMEM));
    memset(&EXMEM[1], 0, sizeof(Latch_EXMEM));
    memset(&MEMWB[0], 0, sizeof(Latch_MEMWB));
    memset(&MEMWB[1], 0, sizeof(Latch_MEMWB));
}

void LAT_update(){
    if (IFIDwrite == true){
        memcpy(&IFID[1], &IFID[0], sizeof(Latch_IFID));
        memset(&IFID[0], 0, sizeof(Latch_IFID));
    }
    else{ // IFIDwrite == false면 같은 명령어를 한번 더 decode한다.
        memcpy(&IFID[0], &IFID[1], sizeof(Latch_IFID));
    }
    if (IDEXwrite == true){
        memcpy(&IDEX[1], &IDEX[0], sizeof(Latch_IDEX));
        memset(&IDEX[0], 0, sizeof(Latch_IDEX));
    }
    else{ // IDEXwrite == false면 같은 명령어를 한번 더 execute하게 하고, IDEX Latch를 잠근다.
        memcpy(&IDEX[0], &IDEX[1], sizeof(Latch_IDEX));
        IDEX[1].valid = false;
    }
    memcpy(&EXMEM[1], &EXMEM[0], sizeof(Latch_EXMEM));
    memset(&EXMEM[0], 0, sizeof(Latch_EXMEM));
    memcpy(&MEMWB[1], &MEMWB[0], sizeof(Latch_MEMWB));
    memset(&MEMWB[0], 0, sizeof(Latch_MEMWB));
}

void set_val_IFID(){
    IFID[0].opcode = IR >> 26;
    IFID[0].rs = (IR & 0x03e00000) >> 21;
    IFID[0].rt = (IR & 0x001f0000) >> 16;
    IFID[0].rd = (IR & 0x0000f800) >> 11;
    IFID[0].shamt = (IR & 0x000007c0) >> 6;
    IFID[0].funct = IR & 0x0000003f;
    IFID[0].imm = IR & 0x0000ffff;
    IFID[0].address = IR & 0x03ffffff;
}

void set_val_IDEX(){
    IDEX[0].nextPC = IFID[1].nextPC;
    IDEX[0].opcode = IFID[1].opcode;
    IDEX[0].rs = IFID[1].rs;
    IDEX[0].rt = IFID[1].rt;
    IDEX[0].rd = IFID[1].rd;
    IDEX[0].shamt = IFID[1].shamt;
    IDEX[0].funct = IFID[1].funct;
    IDEX[0].address = IFID[1].address;
    IDEX[0].imm = MUX(IDEX[0].sig.ZeroEx, signExtend(IFID[1].imm), zeroExtend(IFID[1].imm));
}

void set_sig_EXMEM(){
    EXMEM[0].sig.JumpAL = IDEX[1].sig.JumpAL;
    EXMEM[0].sig.Branch = IDEX[1].sig.Branch; // 출력용으로만 필요, 이론적으로는 전달X
    EXMEM[0].sig.BranchNE = IDEX[1].sig.BranchNE; // 출력용으로만 필요, 이론적으로는 전달X
    EXMEM[0].sig.RegWrite = IDEX[1].sig.RegWrite;
    EXMEM[0].sig.MemWrite = IDEX[1].sig.MemWrite;
    EXMEM[0].sig.MemRead = IDEX[1].sig.MemRead;
    EXMEM[0].sig.MemtoReg = IDEX[1].sig.MemtoReg;
    EXMEM[0].sig.LoadU = IDEX[1].sig.LoadU;
    EXMEM[0].sig.MemSize = IDEX[1].sig.MemSize;
}

void set_val_EXMEM(){
    EXMEM[0].nextPC = IDEX[1].nextPC;
    EXMEM[0].rd = MUX(IDEX[1].sig.JumpAL, MUX(IDEX[1].sig.RegDst, IDEX[1].rt, IDEX[1].rd), ra); 
}

void set_sig_MEMWB(){
    MEMWB[0].sig.JumpAL = EXMEM[1].sig.JumpAL;
    MEMWB[0].sig.RegWrite = EXMEM[1].sig.RegWrite;
    MEMWB[0].sig.MemtoReg = EXMEM[1].sig.MemtoReg;
    MEMWB[0].sig.LoadU = EXMEM[1].sig.LoadU;
    MEMWB[0].sig.MemSize = EXMEM[1].sig.MemSize;
}

void set_val_MEMWB(){
    MEMWB[0].nextPC = EXMEM[1].nextPC;
    MEMWB[0].ALUResult = EXMEM[1].ALUResult;
    MEMWB[0].rd = EXMEM[1].rd;
}