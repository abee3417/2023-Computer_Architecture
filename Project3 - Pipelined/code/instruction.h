#pragma once

/* Define : R-Type Instruction Function Fields */
#define ADD 0x20
#define ADDU 0x21
#define AND 0x24
#define JR 0x08
#define JALR 0x09
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
#define LB 0x20
#define LBU 0x24
#define LH 0x21
#define LHU 0x25
#define LW 0x23
#define LUI 0x0f
#define ORI 0x0d
#define SLTI 0x0a
#define SLTIU 0x0b
#define SB 0x28
#define SH 0x29
#define SW 0x2b

// 미구현 Instruction : FP관련, HI/LO관련, LL, SC

/* Define : J-Type Instruction Opcodes */
#define J 0x02
#define JAL 0x03