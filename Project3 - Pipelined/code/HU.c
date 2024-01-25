#include "base.h"

void HU_init(){
    PCwrite = true;
    IFIDwrite = true;
    IDEXwrite = true;
    DH_DW_counter = 0;
    CH_DW_counter = 0;
}

void HU_operation_opt0(){
    // Detect Control Hazard (case J, JR, JAL, JALR)
    if (IDEX[1].sig.Jump || IDEX[1].sig.JumpR){
        IFID[0].valid = false;
    }

    if (DH_DW_counter > 0){
        DH_DW_counter--;
        if (DH_DW_counter == 0){
            PCwrite = true;
            IFIDwrite = true;
            IDEXwrite = true;
        }
    }
    // Data Hazard
    else if (((EXMEM[0].rd != 0) && (EXMEM[0].sig.RegWrite) && ((EXMEM[0].rd == IDEX[0].rs) || (EXMEM[0].rd == IDEX[0].rt))) ||
            ((MEMWB[0].rd != 0) && (MEMWB[0].sig.RegWrite) && (((EXMEM[0].rd != IDEX[0].rs) && (MEMWB[0].rd == IDEX[0].rs)) ||
            ((EXMEM[0].rd != IDEX[0].rt) && (MEMWB[0].rd == IDEX[0].rt))))){
        printf("\nData Hazard Detected");
        DH_DW_counter = 2;
        PCwrite = false;
        IFIDwrite = false;
        IDEXwrite = false;
    }
    else{
        PCwrite = true;
        IFIDwrite = true;
        IDEXwrite = true;
    }
}

void HU_operation_opt1(){
    // Detect Control Hazard (case J, JR, JAL, JALR)
    if (IDEX[1].sig.Jump || IDEX[1].sig.JumpR){
        IFID[0].valid = false;
    }

    // Detect Load-Use Hazard
    if (IDEX[1].sig.MemRead && (IDEX[1].rt == IFID[1].rs || IDEX[1].rt == IFID[1].rt)){
        printf("\nLoad-Use Hazard Decected");
        PCwrite = false;
        IFIDwrite = false;
        IDEXwrite = false;
    }

    else{
        PCwrite = true;
        IFIDwrite = true;
        IDEXwrite = true;
    }
}