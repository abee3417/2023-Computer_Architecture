#include "base.h"

int main(){
    initialize();

    /* 임시 테스트값 시작 */
    int i = 0;
    PC = 0x27bd0028; // addiu sp, sp, 40
    // PC = 0x27ddfff8; // addiu sp, fp, -8
    // PC = 0xafbe0004; // sw s8, 4(sp)
    // PC = 0x8fbe0004; // lw s8, 4(sp)
    // PC = 0x00621023; // subu v0, v1, v0
    // PC = 0x14620004; // bne v1, v0, 84
    // PC = 0x0043102a; // slt v0, v0, v1
    // PC = 0x08000039; // j e4
    // PC = 0x0c000000; // jal 0
    // PC = 0x03e00008; // jr ra
    /* 임시 테스트값 끝  */
    // PC값이 0xffff:ffff될때까지 Simulator 구동 : while (PC != 0xffffffff)
    while (i < 1){
        /* 1. Fetch
        Instruction is moved from memory to CPU */
        IR = IM_fetch(PC);

        /* 2. Decode
        Instruction decoded -> Identify the operation -> Registers usages */
        decode();
        CU_setting(ins->opcode, ins->funct);
        RF_read(ins->rs, ins->rt);

        /* 3. Execute
        ALU operation is performed and ALU result is produced */
        ins->imm = signExtend(ins->imm);
        ALUControl = ALUCU_control(ALUOp, MUX(RegDst, ins->opcode, ins->funct));
        ALUResult = ALU_calculate(ALUControl, MUX(Shift, ReadData1, ReadData2), MUX(Shift, MUX(ALUSrc, ReadData2, ins->imm), ins->shamt)); // sll(srl)은 rd = rt <<(>>) shamtx
        /* 4. Memory Allocation
        Memory is accessed when load and store */
        DM_operation(MemRead, MemWrite, ALUResult, ReadData2); // ALUResult-address, ReadData2-writeData

        /* 5. Write Back
        Destination registers are updated including PC */
        RF_write(RegWrite, MUX(JumpAL, MUX(RegDst, ins->rt, ins->rd), ra), MUX(JumpAL, MUX(MemtoReg, ALUResult, ReadData), PCAdder(PC))); // jal은 ra에 pc+8 저장하고 jump
        // BEQ : Branch == 1, ALUResult == 0
        // BNE : BranchNE == 1, ALUResult != 0
        PCSrc = ((Branch && !ALUResult) || (BranchNE && ALUResult));
        PC = MUX(Jump, MUX(PCSrc, MUX(JumpR, PCAdder(PC), ReadData1), branchAdder(PC, ins->imm)), jumpMaker(PC, ins->address));

        // Show current state.
        state();
        i++;
    }
    return 0;
}

void initialize(){
    PC = 0x00000000;
    IR = 0x00000000;
    ins = (Instruction*)malloc(sizeof(Instruction));
    IM_init();
    RF_init();
    DM_init();
    CU_init();
}

void decode(){
    ins->opcode = IR >> 26;
    ins->rs = (IR & 0x03e00000) >> 21;
    ins->rt = (IR & 0x001f0000) >> 16;
    ins->rd = (IR & 0x0000f800) >> 11;
    ins->shamt = (IR & 0x000007c0) >> 6;
    ins->funct = IR & 0x0000003f;
    ins->imm = IR & 0x0000ffff;
    ins->address = IR & 0x03ffffff;
}

uint32_t signExtend(uint16_t imm){
    // 0x8000과 AND연산을 하면 최상위비트가 1인지 0인지를 알 수 있다.
    if (imm & 0x8000){ // 최상위비트가 1 : 음수일 경우 1로 패딩
        return (0xffff0000 | imm);
    }
    else{ // 최상위비트가 0 : 양수일 경우 0으로 패딩
        return (0x0000ffff & imm);
    }
}

uint32_t PCAdder(uint32_t PC){
    return PC + MUX(JumpAL, 4, 8);
}

uint32_t branchAdder(uint32_t PC, uint32_t imm){
    return PC + 4 + (imm << 2);
}

uint32_t jumpMaker(uint32_t PC, uint32_t address){
    return ((PC + 4) & 0xf0000000) | (address << 2);
}
uint32_t MUX(bool sign, uint32_t input1, uint32_t input2){
    if (sign == 0){
        return input1;
    }
    else{
        return input2;
    }
}

void state(){
    /* (미완) 현재 상태를 출력하는 함수 만들기 */
    printf("========== Instruction ==========\n");
    printf("opcode : %d\n", ins->opcode);
    printf("rs : %d\n", ins->rs);
    printf("rt : %d\n", ins->rt);
    printf("rd : %d\n", ins->rd);
    printf("shamt : %d\n", ins->shamt);
    printf("funct : %d\n", ins->funct);
    printf("imm : %d\n", ins->imm);
    printf("address : %d\n", ins->address);
    printf("========== CU Signal ==========\n");
    printf("Jump : %d\n", Jump);
    printf("JumpR : %d\n", JumpR);
    printf("JumpAL : %d\n", JumpAL);
    printf("Branch : %d\n", Branch);
    printf("BranchNE : %d\n", BranchNE);
    printf("Shift : %d\n", Shift);
    printf("RegDst : %d\n", RegDst);
    printf("RegWrite : %d\n", RegWrite);
    printf("MemWrite : %d\n", MemWrite);
    printf("MemRead : %d\n", MemRead);
    printf("MemtoReg : %d\n", MemtoReg);
    printf("ALUSrc : %d\n", ALUSrc);
    printf("ALUOp : %d\n", ALUOp);
    printf("ALUSrc : %d\n", PCSrc);
    printf("========== Data ==========\n");
    printf("ALUControl : %d\n", ALUControl);
    printf("ALUResult : %d\n", ALUResult);
    printf("ReadData1 : %d\n", ReadData1);
    printf("ReadData2 : %d\n", ReadData2);
    printf("ReadData : %d\n", ReadData);
    printf("========== Registers ==========\n");
    printf("PC : 0x%08x\n", PC);
    printf("IR : 0x%08x\n", IR);
    for (int i = 0; i < 32; i ++){
        printf("Register[%d] : %d\n", i, Register[i]);
    }
}