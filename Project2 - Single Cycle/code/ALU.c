/* ALU C Code */
#include "base.h"

uint32_t ALU_calculate(uint8_t control, uint32_t op1, uint32_t op2){
    switch (control){
        case 0b0000:
            return op1 & op2;
        case 0b0001:
            return op1 | op2;
        case 0b1100:
            return ~(op1 | op2);
        case 0b0010:
            return op1 + op2;
        case 0b0110:
            return op1 - op2;
        case 0b0111:
            return op1 < op2;
        case 0b1000:
            return op2 << 16;
        case 0b1010:
            return op1 << op2;
        case 0b1110:
            return op1 >> op2;
    }
}