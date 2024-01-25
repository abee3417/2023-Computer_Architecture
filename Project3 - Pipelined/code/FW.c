#include "base.h"

void FW_init(){
    fwA = 0b00;
    fwB = 0b00;
}

void FW_operation(){
    fwA = 0b00;
    fwB = 0b00;
    if ((EXMEM[0].rd != 0) && (EXMEM[0].sig.RegWrite)){ // EXMEM forwarding (add 등)
        if (EXMEM[0].rd == IDEX[0].rs){
            fwA = 0b10;
        }
        if (EXMEM[0].rd == IDEX[0].rt){
            fwB = 0b10;
        }
    }
    if ((MEMWB[0].rd != 0) && (MEMWB[0].sig.RegWrite)){ // MEMWB forwarding (LW 등)
        if ((EXMEM[0].rd != IDEX[0].rs) && (MEMWB[0].rd == IDEX[0].rs)){
            fwA = 0b01;
        }
        if ((EXMEM[0].rd != IDEX[0].rt) && (MEMWB[0].rd == IDEX[0].rt)){
            fwB = 0b01;
        }
    }
}