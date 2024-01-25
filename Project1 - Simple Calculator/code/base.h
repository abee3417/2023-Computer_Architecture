#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Define : Opcode Macro Numbers */
#define ADD 0x414444 // add r r r
#define SUB 0x535542 // sub r r r
#define MUL 0x4d554c // mul r r r
#define DIV 0x444956 // div r r r
#define MOV 0x4d4f56 // mov r r/imm
#define LW 0x004c57 // lw r imm
#define SW 0x005357 // sw r stdout
#define RST 0x525354 // rst
#define JMP 0x4a4d50 // jmp imm
#define BEQ 0x424551 // beq r/imm r/imm imm
#define BNE 0x424e45 // bne r/imm r/imm imm
#define SLT 0x534c54 // slt r r/imm r/imm

/* Define : Operand Type */
#define r0 0x5230
#define r1 0x5231
#define r2 0x5232
#define r3 0x5233
#define r4 0x5234
#define r5 0x5235
#define r6 0x5236
#define r7 0x5237
#define r8 0x5238
#define r9 0x5239
#define imm 0x5240
#define SWOp 0x5241

/* Define : Other Numbers */
#define MAX_STORE 100
#define MAX_LINE 100
#define strImm 0x3058
#define STDO 0x5354444f
#define DOUT 0x444f5554

/* Operand and Instruction Formats */
typedef struct Operand{
    uint32_t type;
    int val;
}Operand;

typedef struct Instruction{
    uint32_t opcode;
    Operand op1;
    Operand op2;
    Operand op3;
    char* printIns[4];
}Instruction;

/* Variables */
int limit; // memory에 실질적으로 담겨져있는 명령어 수
int PC; // Program Counter
int R[10] = { 0, }; // r0 ~ r9 레지스터, 초기값은 0으로 모두 설정
int R_Buffer[10] = { 0, }; // r0 ~ r9 상태변환 체크용 배열로 1인것들만 출력하게 한다.
char memory[MAX_STORE][MAX_LINE]; // input.txt의 명령어를 저장할 memory
bool isErr_NAME; // Name Error 신호
bool isErr_FEW; // Few Operand Error 신호
bool isErr_MORE; // More Operand Error 신호
FILE* fp; // input.txt

/* Functions */
void state(); // r0 ~ r9의 상태를 출력
void read(); // input 파일을 불러와서 내용을 가져온다.
Instruction* decode(); // 명령어 1줄을 해석해서 실행을 위한 ins 구조체로 만들고 return해준다.
uint32_t decode_type(char* str); // 문자열을 받아서 opcode나 type으로 반환해준다.
int decode_val(uint32_t flag, char* str); // operand의 val값을 반환해준다.
int getCnt(uint32_t opcode); // 각 명령어별로 필요한 operand의 갯수를 반환해준다.
void execute(Instruction* ins); // decode해서 받은 ins로 명령을 실행한다.
int check_Err(); // 실행 전 발생하는 에러를 체크해준다.
bool isReg(uint32_t flag); // 레지스터면 true, 아니면 false을 반환해준다.
bool isErr_1(uint32_t opcode, uint32_t op1_type); // operand 1개 전용 에러체크함수
bool isErr_2(uint32_t opcode, uint32_t op1_type, uint32_t op2_type); // operand 2개 전용 에러체크함수
bool isErr_3(uint32_t opcode, uint32_t op1_type, uint32_t op2_type, uint32_t op3_type); // operand 3개 전용 에러체크함수