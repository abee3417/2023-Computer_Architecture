#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Define : Register Numbers */
#define zero 0
#define at 1
#define v0 2
#define v1 3
#define a0 4
#define a1 5
#define a2 6
#define a3 7
#define t0 8
#define t1 9
#define t2 10
#define t3 11
#define t4 12
#define t5 13
#define t6 14
#define t7 15
#define s0 16
#define s1 17
#define s2 18
#define s3 19
#define s4 20
#define s5 21
#define s6 22
#define s7 23
#define t8 24
#define t9 25
#define k0 26
#define k1 27
#define gp 28
#define sp 29
#define fp 30
#define ra 31

/* Define : R-Type Instruction Function Fields */
#define ADD 0x20
#define ADDU 0x21
#define AND 0x24
#define JR 0x08
#define NOR 0x27
#define OR 0x25
#define SLT 0x2a
#define SLTU 0x2b
#define SLL 0x00
#define SRL 0x02
#define SUB 0x22
#define SUBU 0x23

/* Define : I-Type Instruction Opcodes */
#define ADDI 0x08
#define ADDIU 0x09
#define ANDI 0x0c
#define BEQ 0x04
#define BNE 0x05
#define LBU 0x24
#define LHU 0x25
#define LL 0x30
#define LUI 0x0f
#define LW 0x23
#define ORI 0x0d
#define SLTI 0x0a
#define SLTIU 0x0b
#define SB 0x28
#define SC 0x38
#define SH 0x29
#define SW 0x2b

/* Define : J-Type Instruction Opcodes */
#define J 0x02
#define JAL 0x03

/* Define : Other Numbers */
#define MAX_SIZE 100000

/* Instruction Formats */
typedef struct Instruction{
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
    uint32_t imm; // extend한 경우까지 고려
    uint32_t address;
}Instruction;


/* Control Unit Components */
// Functions
void CU_init(); // 신호들의 값을 0으로 초기화해준다.
void CU_setting(uint8_t opcode, uint8_t funct); // opcode에 따라 CU의 신호들을 설정한다.
// Variables
bool Jump;
bool JumpR;
bool JumpAL;
bool Branch;
bool BranchNE;
bool Shift;
bool RegDst;
bool RegWrite;
bool MemWrite;
bool MemRead;
bool MemtoReg;
bool ALUSrc;
bool PCSrc;
uint8_t ALUOp;


/* Instruction Memory Components and related elements*/
// Functions
void IM_init(); // bin파일에서 데이터를 읽어온다.
uint32_t IM_fetch(uint32_t pc); // pc로 명령어를 찾아 return한다.
// Variables
uint32_t InsMemory[MAX_SIZE];
uint32_t PC;
uint32_t IR;
Instruction* ins; // 만들어낸 Instruction


/* Register File Components */
// Functions
void RF_init(); // 레지스터들의 값을 초기화해준다.
void RF_read(uint8_t rs, uint8_t rt); // Read Data 1, 2값을 설정해준다.
void RF_write(bool RegWrite, uint8_t write_r, uint32_t write_data); // write register에 data를 저장한다.
// Variables
uint32_t Register[32];
uint32_t ReadData1;
uint32_t ReadData2;
uint32_t WriteData;


/* Data Memory Components */
// Functions
void DM_init(); // 메모리의 값을 0으로 초기화해준다.
void DM_operation(bool MemRead, bool MemWrite, uint32_t address, uint32_t writeData); // 메모리 접근을 관리한다.
// Variables
uint32_t DataMemory[MAX_SIZE];
uint32_t ReadData;


/* ALU Control Unit and ALU Components */
// Functions
uint8_t ALUCU_control(uint8_t ALUOp, uint8_t flagCode); // 4비트의 control 신호를 만든다. MUX로 flagcode에 funct or opcode가 들어간다.
uint32_t ALU_calculate(uint8_t control, uint32_t op1, uint32_t op2); // ALU Result를 계산한다.
// Variables
uint8_t ALUControl; // ALU로 보내는 4비트 신호
uint32_t ALUResult; // ALU 연산결과값


/* Functions in main.c */
void initialize(); // cpu 내의 부품들을 초기화해준다.
void decode(); // 명령어 32비트로 Instruction을 만든다.
uint32_t signExtend(uint16_t imm); // 16비트 상수를 32비트로 확장해준다.
uint32_t PCAdder(uint32_t PC); // JAL 신호에 따라 PC에 4를 더할지 8을 더할지 결정해준다.
uint32_t branchAdder(uint32_t PC, uint32_t imm); // branch시의 address를 만들어준다.
uint32_t jumpMaker(uint32_t PC, uint32_t address); // jump시의 address를 만들어준다.
uint32_t MUX(bool sign, uint32_t input1, uint32_t input2); // 2:1 MUX
void state(); // 현재 상황 출력