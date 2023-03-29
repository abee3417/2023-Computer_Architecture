/* ALU Control Unit C Code */
#include "base.h"

uint8_t ALUCU_control(uint8_t ALUOp, uint8_t flagCode){
    switch(ALUOp){
        case 0b00:
            return 0b0010;
        case 0b01:
            return 0b0110;
        case 0b10:
            switch(flagCode){
                case AND:
                    return 0b0000;
                case OR:
                    return 0b0001;
                case NOR:
                    return 0b1100;
                case ADD:
                    return 0b0010;
                case ADDU:
                    return 0b0010;
                case SUB:
                    return 0b0110;
                case SUBU:
                    return 0b0110;
                case SLT:
                    return 0b0111;
                case SLTU:
                    return 0b0111;
                case SLL:
                    return 0b1010;
                case SRL:
                    return 0b1110;
            }
        case 0b11:
            switch(flagCode){
                case ANDI:
                    return 0b0000;
                case ORI:
                    return 0b0001;
                case ADDI:
                    return 0b0010;
                case ADDIU:
                    return 0b0010;
                case SLTI:
                    return 0b0111;
                case SLTIU:
                    return 0b0111;
                case LUI:
                    return 0b1000;
            }
    }
}