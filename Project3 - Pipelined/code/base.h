#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "register.h"
#include "instruction.h"
#define MAX_SIZE 0x1000000

/* HU/FW Control Signals */
bool PCwrite; // PC write
bool IFIDwrite; // IFID write
bool IDEXwrite; // IDEX write
uint8_t fwA; // forward A
uint8_t fwB; // forward B


/* EX Control Signals */
typedef struct Signal_IDEX{
    bool Jump;
    bool JumpR;
    bool JumpAL;
    bool Branch;
    bool BranchNE;
    bool ZeroEx;
    bool Shift;
    bool RegDst;
    bool RegWrite;
    bool MemWrite;
    bool MemRead;
    bool MemtoReg;
    bool LoadU;
    bool ALUSrc;
    uint8_t ALUOp;
    uint8_t MemSize;
}Signal_IDEX;

/* MEM Control Signals */
typedef struct Signal_EXMEM{
    bool JumpAL;
    bool Branch; // 출력용으로만 필요
    bool BranchNE; // 출력용으로만 필요
    bool RegWrite;
    bool MemWrite;
    bool MemRead;
    bool MemtoReg;
    bool LoadU;
    uint8_t MemSize;
}Signal_EXMEM;

/* WB Control Signals */
typedef struct Signal_MEMWB{
    bool JumpAL;
    bool RegWrite;
    bool MemtoReg;
    bool LoadU;
    uint8_t MemSize;
}Signal_MEMWB;

/* 특정 Signal로 만들어지는 Variables */
bool PCSrc; // BranchNE와 Branch로 만드는 신호
bool DataSrc; // MemSize로 만드는 신호

/* IF/ID Latch Formats */
typedef struct Latch_IFID{
    bool valid; // 접근 허용/거절 bit
    uint32_t nextPC;
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
    uint32_t imm;
    uint32_t address;
}Latch_IFID;

/* ID/EX Latch Formats */
typedef struct Latch_IDEX{
    bool valid; // 접근 허용/거절 bit
    uint32_t nextPC;
    uint32_t ReadData1;
    uint32_t ReadData2;
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
    uint32_t imm;
    uint32_t address;
    Signal_IDEX sig;
}Latch_IDEX;

/* EX/MEM Latch Formats */
typedef struct Latch_EXMEM{
    bool valid; // 접근 허용/거절 bit
    uint32_t nextPC;
    uint32_t ReadData1;
    uint32_t ReadData2;
    uint32_t ALUResult;
    uint8_t rd;
    Signal_EXMEM sig;
}Latch_EXMEM;

/* MEM/WB Latch Formats */
typedef struct Latch_MEMWB{
    bool valid; // 접근 허용/거절 bit
    uint32_t nextPC;
    uint32_t ReadData;
    uint32_t ALUResult;
    uint8_t rd;
    Signal_MEMWB sig;
}Latch_MEMWB;

/* Instruction Memory */
void IM_init();
uint32_t IM_fetch(uint32_t address);

/* Register File */
void RF_init();
void RF_read(uint8_t rs, uint8_t rt);
void RF_write(bool RegWrite, uint8_t write_r, uint32_t write_data);

/* Control Unit */
void CU_init();
void CU_setting(uint8_t opcode, uint8_t funct);

/* ALU Control Unit and ALU */
uint8_t ALUCU_control(uint8_t ALUOp, uint8_t flagCode); // 4비트의 control 신호를 만든다. MUX로 flagcode에 funct or opcode가 들어간다.
uint32_t ALU_calculate(uint8_t control, uint32_t op1, uint32_t op2); // ALU Result를 계산한다.

/* Data Memory */
void DM_init(); // 메모리의 값을 0으로 초기화해준다.
void DM_operation(bool MemRead, bool MemWrite, uint32_t address, uint32_t writeData, int size); // 메모리 접근을 관리한다.

/* Latches */
void LAT_init();
void LAT_update();
void set_val_IFID();
void set_val_IDEX();
void set_sig_EXMEM();
void set_val_EXMEM();
void set_sig_MEMWB();
void set_val_MEMWB();

/* Forward Unit */
void FW_init();
void FW_operation();

/* Hazard Detection Unit */
int DH_DW_counter;
void HU_init();
void HU_operation_opt0(); // detect & wait일 때 HU unit
void HU_operation_opt1(); // forwarding일 때 HU unit

/* Option Variable */
char forward_OPT;
char predict_OPT;

/* Branch Detect and Wait */
int CH_DW_counter;
void branch_DW();
void branch_operation();

/* Always Not Taken Branch Predictor */
void ANT_prediction();
void ANT_operation();

/* Always Taken Branch Predictor */
uint32_t ALT_backupPC;
void ALT_prediction();
void ALT_operation();

/* Variables to print log */
int cycle;
int R_cnt;
int I_cnt;
int J_cnt;
int brc_cnt;
int success;
int fail;
int mem_cnt;


/* Other Logic Circuit */
uint32_t signExtend(uint16_t imm);
uint32_t zeroExtend(uint16_t imm);
uint32_t PCAdder(uint32_t PC);
uint32_t branchAdder(uint32_t PC, uint32_t imm); // branch시의 address 생성
uint32_t jumpMaker(uint32_t PC, uint32_t address); // jump시의 address 생성
uint32_t MUX(bool sign, uint32_t input1, uint32_t input2); // 2:1 MUX
uint32_t MUX3(uint8_t sign, uint32_t input1, uint32_t input2, uint32_t input3); // 3:1 MUX

/* Latch Variabels */
Latch_IFID IFID[2];
Latch_IDEX IDEX[2];
Latch_EXMEM EXMEM[2];
Latch_MEMWB MEMWB[2];

/* Essential Components */
uint8_t InsMemory[MAX_SIZE];
uint8_t DataMemory[MAX_SIZE];
uint32_t Register[32];
uint32_t PC;
uint32_t IR;

/* Functions in MAIN.c */
void start(); // cpu 내의 부품들을 초기화
void IF();
void ID();
void EX();
void MEM();
void WB();
void print_cycle(); // fetch 후의 상황 출력
void print_result(); // 현재 상황 출력