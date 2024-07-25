/*
 * apex_cpu.c
 * Contains APEX cpu pipeline implementation
 *
 * Author:
 * Copyright (c) 2020, Gaurav Kothari (gkothar1@binghamton.edu)
 * State University of New York at Binghamton
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "apex_cpu.h"
#include "apex_macros.h"

/* Converts the PC(4000 series) into array index for code memory
 *
 * Note: You are not supposed to edit this function
 */
static const char *
reset_opcode_str(int opcode)
{
    //printf("opcode here %d",opcode);
    if (opcode == OPCODE_ADD)
    {
        return "ADD";
    }

    if (opcode == OPCODE_SUB)
    {
        return "SUB";
    }
    if (opcode == OPCODE_ADDL)
    {
        return "ADDL";
    }
    if (opcode == OPCODE_SUBL)
    {
        return "SUBL";
    }        

    if (opcode == OPCODE_MUL)
    {
        return "MUL";
    }
    if (opcode == OPCODE_DIV)
    {
        return "DIV";
    }
    if (opcode == OPCODE_AND)
    {
        return "AND";
    }
    if (opcode == OPCODE_OR)
    {
        return "OR";
    }
    if (opcode == OPCODE_XOR)
    {
        return "XOR";
    }
    if (opcode == OPCODE_MOVC)
    {
        return "MOVC";
    }                    
    if (opcode == OPCODE_LOAD)
    {
        return "LOAD";
    }
    if (opcode == OPCODE_STORE)
    {
        return "STORE";
    }    

    if (opcode == OPCODE_BZ)
    {
        return "BZ";
    } 
    if (opcode == OPCODE_BNZ)
    {
        return "BNZ";
    }     

    if (opcode == OPCODE_HALT)
    {
        return "HALT";
    }
    if (opcode == OPCODE_CMP)
    {
        return "CMP";
    } 
    if (opcode == OPCODE_LDR)
    {
        return "LDR";
    }
    if (opcode == OPCODE_STR)
    {
        return "STR";
    }                  
    if (opcode == OPCODE_NOP)
    {
        return "NOP";
    }   
    if (opcode == OPCODE_JUMP)
    {
        return "JUMP";
    }         
    return 0;
}


static int
get_code_memory_index_from_pc(const int pc)
{
    return (pc - 4000) / 4;
}
int front = 0;
int rear = -1;
int itemCount = 0;

//QUEUE Implementation methods
int peek(const APEX_CPU *cpu) {
   return cpu->freeListQueue[front];
}

bool isEmpty(const APEX_CPU *cpu) {
   return itemCount == 0;
}

bool isFull(const APEX_CPU *cpu) {
   return itemCount == FREELIST_QUEUE_SIZE;
}

int size(const APEX_CPU *cpu) {
   return itemCount;
} 

void insert(APEX_CPU *cpu,int data) {

   if(!isFull(cpu)) {
	
      if(rear == FREELIST_QUEUE_SIZE-1) {
         rear = -1;            
      }       
      cpu->freeListQueue[++rear] = data;
      itemCount++;
   }
}

int removeData(const APEX_CPU *cpu) {
   int data = cpu->freeListQueue[front++];
	
   if(front == FREELIST_QUEUE_SIZE) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}

//Queue implementation complete


//Instruction Queue implementation
int iQCount = 0;
int rearIQ = 0;

bool isIQEmpty(const APEX_CPU *cpu) {
   return iQCount == 0;
}

bool isIQFull(const APEX_CPU *cpu) {
   return iQCount == Instruction_QUEUE_SIZE;
}

bool sourceRegisterInInsQueue(const APEX_CPU *cpu,int sourceRegister) {
   bool result = false;
   for (int i = 0; i < Instruction_QUEUE_SIZE; i++)
   {
    if(cpu->insQueue[i].rd == sourceRegister){
        result = true;
    }
   }
   
   return result;
}

void print_ins_queue(const APEX_CPU *cpu){
    for (int i = 0; i < iQCount; i++)
    {
        /* code */
        printf("instruction Queue destination register %d\n",cpu->insQueue[i].rd);
    }
    
}

int sizeIQ(const APEX_CPU *cpu) {
   return iQCount;
} 

int insertIQ(APEX_CPU *cpu) {
      cpu->insQueue[rearIQ] = cpu->newInsQueueEntry;
      int result = rearIQ;
      rearIQ++;
      iQCount++;
      return result;
}

void printINSQ(APEX_CPU *cpu){
    printf("Instruction Queue Entries \n");
    for (int i = 0; i < rearIQ; i++)
    {
        printf("%s RS1Valid(%d) RS1Tag(%d) RS1Value(%d) RS2Valid(%d) RS2Tag(%d) RS2Value(%d) RS3Valid(%d) RS3Tag(%d) RS3Value(%d) RD(%d) LiteralValue(%d) \n",
        reset_opcode_str(cpu->insQueue[i].opcode),cpu->insQueue[i].rs1_valid,cpu->insQueue[i].rs1_tag,cpu->insQueue[i].rs1_value
        ,cpu->insQueue[i].rs2_valid,cpu->insQueue[i].rs2_tag,cpu->insQueue[i].rs2_value
        ,cpu->insQueue[i].rs3_valid,cpu->insQueue[i].rs3_tag,cpu->insQueue[i].rs3_value,cpu->insQueue[i].rd,cpu->insQueue[i].imm);

    }
    printf("=========================================\n");        
    
}

Instruction_Queue removeDataIQ(APEX_CPU *cpu, int index) {
   Instruction_Queue data = cpu->insQueue[index];
	
//    if(front == FREELIST_QUEUE_SIZE) {
//       front = 0;
//    }

   for (int i = (index+1); i < Instruction_QUEUE_SIZE; i++)
   {
    /* code */
    cpu->insQueue[i-1] = cpu->insQueue[i];
   }
   rearIQ--; 
   iQCount--;
   return data;  
}
//End of Instruciton queue implementation


//Waiting Queue implementation
int wQCount = 0;
int rearWQ = 0;
int frontWQ = 0;

bool isWQEmpty() {
   return wQCount == 0;
}

bool isWQFull(const APEX_CPU *cpu) {
   return wQCount == Waiting_QUEUE_SIZE;
}

void print_waiting_queue(const APEX_CPU *cpu){
    for (int i = 0; i < wQCount; i++)
    {
        /* code */
        //printf("Waiting Queue %d",cpu->insQueue[i].rd);
    }
    
}

int sizeWQ(const APEX_CPU *cpu) {
   return wQCount;
} 

void insertWQ(APEX_CPU *cpu, int stage) {
    if(!isWQFull(cpu)) {
      cpu->waitingQueue[rearWQ] = stage;
      rearWQ++;
      wQCount++;        
    }
}

int removeStage(const APEX_CPU *cpu) {
   int data = cpu->waitingQueue[frontWQ++];
	
   if(frontWQ == Waiting_QUEUE_SIZE) {
      frontWQ = 0;
   }
	
   wQCount--;
   return data;  
}


//End of Waiting queue implementation


//ROB implementation
int robElementCount = 0;
int robHead = 0;
int robTail = 0;

bool isROBEmpty(){
    return (robElementCount==0);
}

bool isROBFull(){
    return (robElementCount == ROB_SIZE);
}
void insertROB(APEX_CPU *cpu){
    cpu->rob[robTail] = cpu->newROBEntry;
    robElementCount++;
    robTail = (robTail + 1) % ROB_SIZE;
}

ROB dequeueROB(APEX_CPU *cpu){
    ROB result = cpu->rob[robHead];
    robHead = (robHead + 1) % ROB_SIZE;
    robElementCount--;  
    return result;
}

ROB removeTailROB(APEX_CPU *cpu){
    ROB result = cpu->rob[robTail];
    // robHead = (robHead + 1) % ROB_SIZE;
    // robElementCount--;  
    if(robHead == robTail){
        robHead=0;
        robTail=0;
        robElementCount=0;
    }else{
        robElementCount--;
        robTail = (robTail - 1) % ROB_SIZE;
    }
    return result;
}

void printROB(APEX_CPU *cpu){
    printf("ROB Entries \n");
    if(robTail >= robHead)
    {
     
        // Loop to print elements from
        // front to rear.
        for(int i = robHead; i < robTail; i++)
        {       
            printf("%s PC(%d) PD(%d) PdValid(%d) RD(%d) OverwrittenRenameTableEntry(%d) \n",reset_opcode_str(cpu->rob[i].instructionOpcode),
            cpu->rob[i].pc,cpu->rob[i].pd,cpu->rob[i].pdValidBit,cpu->rob[i].rd,cpu->rob[i].overwrittenRenameTableEntry);
        }
    }
 
    // If rear crossed the max index and
    // indexing has started in loop
    else
    {      
        // Loop for printing elements from
        // front to max size or last index
        for(int i = robHead; i < ROB_SIZE; i++)
        {          
            printf("%s PC(%d) PD(%d) PdValid(%d) RD(%d) OverwrittenRenameTableEntry(%d) \n",reset_opcode_str(cpu->rob[i].instructionOpcode),
            cpu->rob[i].pc,cpu->rob[i].pd,cpu->rob[i].pdValidBit,cpu->rob[i].rd,cpu->rob[i].overwrittenRenameTableEntry);     
        }
 
        // Loop for printing elements from
        // 0th index till rear position
        for(int i = 0; i < robTail; i++)
        {   
            printf("%s PC(%d) PD(%d) PdValid(%d) RD(%d) OverwrittenRenameTableEntry(%d) \n",reset_opcode_str(cpu->rob[i].instructionOpcode),
            cpu->rob[i].pc,cpu->rob[i].pd,cpu->rob[i].pdValidBit,cpu->rob[i].rd,cpu->rob[i].overwrittenRenameTableEntry);
        }
    }
printf("=========================================\n");        
}
//End of ROB Implementation


//LSQ implementation
int lsqElementCount = 0;
int lsqHead = 0;
int lsqTail = 0;

bool isLSQEmpty(){
    return (lsqElementCount==0);
}

bool isLSQFull(){
    return (lsqElementCount == LSQ_SIZE);
}
void insertLSQ(APEX_CPU *cpu){
    cpu->lsq[lsqTail] = cpu->newLSQEntry;
    lsqElementCount++;
    lsqTail = (lsqTail + 1) % LSQ_SIZE;
}

LSQ dequeueLSQ(APEX_CPU *cpu){
    LSQ result = cpu->lsq[lsqHead];
    lsqHead = (lsqHead + 1) % LSQ_SIZE;
    lsqElementCount--;  
    return result;
}
bool sourceRegisterInLSQ(const APEX_CPU *cpu,int sourceRegister){
    int result;
    if(lsqTail >= lsqHead)
    {
     
        // Loop to print elements from
        // front to rear.
        for(int i = lsqHead; i < lsqTail; i++)
        {
            if(cpu->lsq[i].instructionOpcode == OPCODE_LOAD && cpu->lsq[i].rd == sourceRegister){
                result = true;
            }                 
        }
    }
 
    // If rear crossed the max index and
    // indexing has started in loop
    else
    {      
        // Loop for printing elements from
        // front to max size or last index
        for(int i = lsqHead; i < LSQ_SIZE; i++)
        {
            if(cpu->lsq[i].instructionOpcode == OPCODE_LOAD && cpu->lsq[i].rd == sourceRegister){
                result = true;
            }                    
        }
 
        // Loop for printing elements from
        // 0th index till rear position
        for(int i = 0; i < lsqTail; i++)
        {
            if(cpu->lsq[i].instructionOpcode == OPCODE_LOAD && cpu->lsq[i].rd == sourceRegister){
                result = true;
            }          
        }
    }    
    return result;
}
void printLSQ(APEX_CPU *cpu){
    printf("LSQ Entries \n");
    if(lsqTail >= lsqHead)
    {
        for(int i = lsqHead; i < lsqTail; i++)
        {       
            printf("%s PC(%d) RD(%d) Rs1_Valid(%d) Rs1(%d) Rs1_value(%d) Rs2_Valid(%d) Rs2(%d) Rs2_value(%d) Rs3_Valid(%d) Rs3(%d) Rs3_value(%d) MEM_ADDRESS(%d) \n",
            reset_opcode_str(cpu->lsq[i].instructionOpcode),cpu->lsq[i].pc,cpu->lsq[i].rd,cpu->lsq[i].rs1ValidBit,cpu->lsq[i].rs1,cpu->lsq[i].rs1Value
            ,cpu->lsq[i].rs2ValidBit,cpu->lsq[i].rs2,cpu->lsq[i].rs2Value,cpu->lsq[i].rs3ValidBit,cpu->lsq[i].rs3,cpu->lsq[i].rs3Value,cpu->lsq[i].memory_address);
        }
    }
 
    else
    {      
        for(int i = lsqHead; i < LSQ_SIZE; i++)
        {          
            printf("%s PC(%d) RD(%d) Rs1_Valid(%d) Rs1(%d) Rs1_value(%d) Rs2_Valid(%d) Rs2(%d) Rs2_value(%d) Rs3_Valid(%d) Rs3(%d) Rs3_value(%d) MEM_ADDRESS(%d) \n",
            reset_opcode_str(cpu->lsq[i].instructionOpcode),cpu->lsq[i].pc,cpu->lsq[i].rd,cpu->lsq[i].rs1ValidBit,cpu->lsq[i].rs1,cpu->lsq[i].rs1Value
            ,cpu->lsq[i].rs2ValidBit,cpu->lsq[i].rs2,cpu->lsq[i].rs2Value,cpu->lsq[i].rs3ValidBit,cpu->lsq[i].rs3,cpu->lsq[i].rs3Value,cpu->lsq[i].memory_address);
        }
 
        for(int i = 0; i < lsqTail; i++)
        {   
            printf("%s PC(%d) RD(%d) Rs1_Valid(%d) Rs1(%d) Rs1_value(%d) Rs2_Valid(%d) Rs2(%d) Rs2_value(%d) Rs3_Valid(%d) Rs3(%d) Rs3_value(%d) MEM_ADDRESS(%d) \n",
            reset_opcode_str(cpu->lsq[i].instructionOpcode),cpu->lsq[i].pc,cpu->lsq[i].rd,cpu->lsq[i].rs1ValidBit,cpu->lsq[i].rs1,cpu->lsq[i].rs1Value
            ,cpu->lsq[i].rs2ValidBit,cpu->lsq[i].rs2,cpu->lsq[i].rs2Value,cpu->lsq[i].rs3ValidBit,cpu->lsq[i].rs3,cpu->lsq[i].rs3Value,cpu->lsq[i].memory_address);
        }
    }
printf("=========================================\n");  
}


LSQ removeTailLSQ(APEX_CPU *cpu){
    LSQ result = cpu->lsq[lsqTail];
    if(lsqHead == lsqTail){
        lsqHead=0;
        lsqTail=0;
        lsqElementCount=0;
    }else{
        lsqElementCount--;
        lsqTail = (lsqTail - 1) % ROB_SIZE;
    }
    return result;
}
//End of LSQ Implementation




static void
print_instruction(const CPU_Stage *stage)
{
    switch (stage->opcode)
    {
        case OPCODE_ADD:
        case OPCODE_CMP:
        {
            printf("%s,R%d,R%d,R%d ", stage->opcode_str, stage->rd, stage->rs1,
                   stage->rs2);
            break;
        }        
        case OPCODE_SUB:
        {
            printf("%s,R%d,R%d,R%d ", stage->opcode_str, stage->rd, stage->rs1,
                   stage->rs2);
            break;
        }
        case OPCODE_ADDL:
        case OPCODE_SUBL:
        case OPCODE_LDR:  
        case OPCODE_STR:        
        case OPCODE_MUL:
        case OPCODE_DIV:
        case OPCODE_NOP:        
        case OPCODE_AND:
        case OPCODE_OR:
        case OPCODE_XOR:
        {
            printf("%s,R%d,R%d,R%d ", stage->opcode_str, stage->rd, stage->rs1,
                   stage->rs2);
            break;
        }
              
        case OPCODE_MOVC:
        {
            printf("%s,R%d,#%d ", stage->opcode_str, stage->rd, stage->imm);
            break;
        }
        case OPCODE_JUMP:
        {
            printf("%s,R%d,#%d ", stage->opcode_str, stage->rs1, stage->imm);
            break;
        }        
        case OPCODE_LOAD:
        {
            printf("%s,R%d,R%d,#%d ", stage->opcode_str, stage->rd, stage->rs1,
                   stage->imm);
            break;
        }

        case OPCODE_STORE:
        {
            printf("%s,R%d,R%d,#%d ", stage->opcode_str, stage->rs1, stage->rs2,
                   stage->imm);
            break;
        }

        case OPCODE_BZ:
        case OPCODE_BNZ:
        {
            printf("%s,#%d ", stage->opcode_str, stage->imm);
            break;
        }

        case OPCODE_HALT:
        {
            printf("%s", stage->opcode_str);
            break;
        }
    }
}

/* Debug function which prints the CPU stage content
 *
 * Note: You can edit this function to print in more detail
 */
static void
print_stage_content(const char *name, const CPU_Stage *stage)
{
    printf("%-15s: pc(%d) ", name, stage->pc);
    print_instruction(stage);
    printf("\n");
}

/* Debug function which prints the register file
 *
 * Note: You are not supposed to edit this function
 */
static void
print_reg_file(const APEX_CPU *cpu)
{
    int i;

    printf("----------\n%s\n----------\n", "Registers:");

    for (int i = 0; i < REG_FILE_SIZE / 2; ++i)
    {
        printf("R%-3d[%-3d] ", i, cpu->regs[i]);
    }

    printf("\n");

    for (i = (REG_FILE_SIZE / 2); i < REG_FILE_SIZE; ++i)
    {
        printf("R%-3d[%-3d] ", i, cpu->regs[i]);
    }

    printf("\n");
}


static void
print_phy_reg_file(const APEX_CPU *cpu)
{
    int i;

    printf("----------\n%s\n----------\n", "Physical Registers:");

    for (int i = 0; i < PHY_REG_FILE_SIZE / 2; ++i)
    {
        printf("R%-3d[%-3d] ", i, cpu->physicalRegisters[i]);
    }

    printf("\n");

    for (i = (PHY_REG_FILE_SIZE / 2); i < PHY_REG_FILE_SIZE; ++i)
    {
        printf("R%-3d[%-3d] ", i, cpu->physicalRegisters[i]);
    }

    printf("\n");
}


static void
print_mem_file(const APEX_CPU *cpu)
{
    int i;

    printf("----------\n%s\n----------\n", "Memory Content:");
    for ( i = 0; i < DATA_MEMORY_SIZE; i++)
    {
        if(cpu->data_memory[i]!=0){
            printf("MEM%-3d[%-3d] ", i, cpu->data_memory[i]);
        }
        /* code */   
    }
    

    printf("\n");
}

/*
 * Fetch Stage of APEX Pipeline
 *
 * Note: You are free to edit this function according to your implementation
 */
static void
APEX_fetch(APEX_CPU *cpu)
{
    APEX_Instruction *current_ins;
    // if (cpu->fetch.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
    //     print_stage_content("Fetch FU", &cpu->fetch);
    // }else{
    //     if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
    //         char name[9] = "Fetch FU";
    //         printf("%-15s:", name);
    //         printf("Empty");
    //         printf("\n");  
    //     }
    // }    
    if (cpu->fetch.has_insn && cpu->fetch_blocked == 0)
    {
        if (cpu->fetch_decode_from_next_cycle == TRUE)
        {
            //cpu->fetch_decode_from_next_cycle = FALSE;
            
            /* Skip this cycle*/
            return;
        }        
        /* This fetches new branch target instruction from next cycle */
        if (cpu->fetch_from_next_cycle == TRUE)
        {
            cpu->fetch_from_next_cycle = FALSE;
            
            /* Skip this cycle*/
            return;
        }

        /* Store current PC in fetch latch */
        cpu->fetch.pc = cpu->pc;

        /* Index into code memory using this pc and copy all instruction fields
         * into fetch latch  */
        current_ins = &cpu->code_memory[get_code_memory_index_from_pc(cpu->pc)];
        strcpy(cpu->fetch.opcode_str, current_ins->opcode_str);
        cpu->fetch.opcode = current_ins->opcode;
        cpu->fetch.rd = current_ins->rd;
        cpu->fetch.rs1 = current_ins->rs1;
        cpu->fetch.rs2 = current_ins->rs2;
        cpu->fetch.rs3 = current_ins->rs3;
        cpu->fetch.imm = current_ins->imm;
        if(cpu->fetch.opcode == OPCODE_JUMP){
            cpu->fetch_blocked = 1;
        }

        /*printf("In Fetch stage %d\n",cpu->fetch.opcode);
        printf("In Fetch stage %d\n",cpu->fetch.rs1);
        printf("In Fetch stage %d\n",cpu->fetch.rs2);*/

        /* Update PC for next instruction */
        cpu->pc += 4;

        /* Copy data from fetch latch to decode latch*/
        cpu->decode = cpu->fetch;

        if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        {
            print_stage_content("Fetch", &cpu->fetch);
        }

        /* Stop fetching new instructions if HALT is fetched */
        if (cpu->fetch.opcode == OPCODE_HALT)
        {
            cpu->fetch.has_insn = FALSE;
        }
    }
}

/*
 * Decode Stage of APEX Pipeline
 *
 * Note: You are free to edit this function according to your implementation
 */
static void
APEX_decode(APEX_CPU *cpu)
{
    int arcRegister;
    if (cpu->decode.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("Decode FU", &cpu->decode);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[9] = "Decode FU";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }    
    if (cpu->decode.has_insn)
    {
        // if (cpu->fetch_from_next_cycle == TRUE)
        // {
        //     cpu->fetch_from_next_cycle = FALSE;
        //     /* Skip this cycle*/
        //     return;
        // }  
        if (cpu->fetch_decode_from_next_cycle == TRUE)
        {
            cpu->fetch_decode_from_next_cycle = FALSE;
            
            /* Skip this cycle*/
            return;
        }              
        //printf("first line of decode stage is %d\n",cpu->decode.rd);
        /* Read operands from register file based on the instruction type */
        switch (cpu->decode.opcode)
        {
            case OPCODE_SUB:
            case OPCODE_LDR:
            case OPCODE_STORE:                        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR:                                    
            case OPCODE_MUL:
            case OPCODE_CMP:            
            case OPCODE_ADD:
            {
                int physicalRegisterSource1=cpu->renameTable[cpu->decode.rs1];
                int physicalRegisterSource2=cpu->renameTable[cpu->decode.rs2];
                //printf("Physical register source 1 and source 2 %d  %d",physicalRegisterSource1,physicalRegisterSource2);
                cpu->decode.rs1_value = cpu->physicalRegisters[physicalRegisterSource1];
                cpu->decode.rs2_value = cpu->physicalRegisters[physicalRegisterSource2];
                cpu->decode.rs1 = physicalRegisterSource1;
                cpu->decode.rs2 = physicalRegisterSource2;
                break;
            }
            case OPCODE_SUBL:
            case OPCODE_LOAD:                        
            case OPCODE_ADDL:
            {
                int physicalRegisterSource1=cpu->renameTable[cpu->decode.rs1];
                cpu->decode.rs1_value = cpu->physicalRegisters[physicalRegisterSource1];
                cpu->decode.rs1 = physicalRegisterSource1;
                // if(cpu->decode.pc == 4052){
                //     printf("4052 decode rs1 value %d\n",cpu->decode.rs1_value);
                // }
                // cpu->decode.imm = cpu->regs[cpu->decode.rs2];
                break;
            }
            case OPCODE_JUMP:
            {
                int physicalRegisterSource1=cpu->renameTable[cpu->decode.rs1];
                cpu->decode.rs1_value = cpu->physicalRegisters[physicalRegisterSource1];
                cpu->decode.rs1 = physicalRegisterSource1;
                // cpu->decode.imm = cpu->regs[cpu->decode.rs2];
                break;
            }                        
            case OPCODE_STR:
            {
                int physicalRegisterSource1=cpu->renameTable[cpu->decode.rs1];
                int physicalRegisterSource2=cpu->renameTable[cpu->decode.rs2];
                int physicalRegisterSource3=cpu->renameTable[cpu->decode.rs3];                
                cpu->decode.rs1_value = cpu->physicalRegisters[physicalRegisterSource1];
                cpu->decode.rs2_value = cpu->physicalRegisters[physicalRegisterSource2];
                cpu->decode.rs3_value = cpu->physicalRegisters[physicalRegisterSource3];   
                cpu->decode.rs1 = physicalRegisterSource1;
                cpu->decode.rs2 = physicalRegisterSource2;
                cpu->decode.rs3 = physicalRegisterSource3;                                                                      
                break;
            }            
            case OPCODE_MOVC:
            {
                /* MOVC doesn't have register operands */
                break;
            }           
        }
        //destination register renaming
        /* Read operands from register file based on the instruction type */
        switch (cpu->decode.opcode)
        {
            case OPCODE_ADD:
            case OPCODE_ADDL:    
            case OPCODE_SUB:
            case OPCODE_SUBL:                     
            case OPCODE_MUL:
            case OPCODE_AND:
            case OPCODE_CMP:
            case OPCODE_OR:
            case OPCODE_XOR:                      
            case OPCODE_LOAD:
            case OPCODE_LDR: 
            case OPCODE_MOVC:
            {
                /* Free List Stalling */
                if(size(cpu) <= 0){
                    cpu->fetch_from_next_cycle = TRUE;                    
                    return;
                }else{
                    int physicalRegister = removeData(cpu);
                    arcRegister = cpu->decode.rd;
                    cpu->decode.arcDest = cpu->decode.rd;
                    cpu->decode.registerRenamed = cpu->renameTable[cpu->decode.rd];
                    cpu->renameTable[cpu->decode.rd] = physicalRegister;
                    //cpu->decode.registerRenamed = physicalRegister;
                    cpu->decode.rd = physicalRegister;
                }
                break;
            }            
            /*Stop*/
            //Store and STR do not have destination register so we dont need        
            // case OPCODE_STORE:
            // {
            //     cpu->decode.rs1_value = cpu->regs[cpu->decode.rs1];     
            //     cpu->decode.rs2_value = cpu->regs[cpu->decode.rs2];                           
            //     break;
            // }
            // case OPCODE_STR:
            // {
            //     cpu->decode.rs1_value = cpu->regs[cpu->decode.rs1];     
            //     cpu->decode.rs2_value = cpu->regs[cpu->decode.rs2];  
            //     cpu->decode.rs3_value = cpu->regs[cpu->decode.rs3];                         
            //     break;
            // }            
            // case OPCODE_CMP:
            // {
            //     cpu->decode.rs1_value = cpu->regs[cpu->decode.rs1];
            //     cpu->decode.rs2_value = cpu->regs[cpu->decode.rs2];
            //     break;
            // }            
        }
            // if(cpu->decode.pc == 4060){
            //     printf("4060 debug outside if  %d %d  %s %d %d\n",cpu->decode.branchLinkAddr,cpu->renameDispatch.rd,cpu->decode.opcode_str,cpu->decode.opcode,OPCODE_BNZ);
            // }        
        if(cpu->decode.opcode == OPCODE_BNZ || cpu->decode.opcode == OPCODE_BZ){
            cpu->decode.branchLinkAddr = cpu->renameDispatch.rd;
            // if(cpu->decode.pc == 4060){
            //     printf("4060 debug %d %d\n",cpu->decode.branchLinkAddr,cpu->renameDispatch.rd);
            // }            
        }        
        // reading the data from the bus
            if(cpu->forwardingBus1.tag == cpu->decode.rs1){
                cpu->decode.rs1_value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->decode.rs1){
                cpu->decode.rs1_value = cpu->forwardingBus2.data;
            }
            if(cpu->forwardingBus1.tag == cpu->decode.rs2){
                cpu->decode.rs2_value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->decode.rs2){
                cpu->decode.rs2_value = cpu->forwardingBus2.data;
            }
            if(cpu->forwardingBus1.tag == cpu->decode.rs3){
                cpu->decode.rs3_value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->decode.rs3){
                cpu->decode.rs3_value = cpu->forwardingBus2.data;
            }        

         cpu->newROBEntry.robEntryEstablishedBit =1;
         cpu->newROBEntry.instructionOpcode =cpu->decode.opcode;
         cpu->newROBEntry.pc =cpu->decode.pc;
         cpu->newROBEntry.pdValidBit = 0;
         cpu->newROBEntry.pd = cpu->decode.rd;
         cpu->newROBEntry.rd = arcRegister;
         cpu->newROBEntry.overwrittenRenameTableEntry = cpu->decode.registerRenamed;
        /* Copy data from decode latch to execute latch*/
        cpu->renameDispatch = cpu->decode;
        cpu->decode.has_insn = FALSE;

        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("Decode/RF", &cpu->decode);
        // }
    }

}
int newInstructionEntryindex=-1;
static void
APEX_renameDispatch(APEX_CPU *cpu)
{
    if (cpu->renameDispatch.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("Rename Dispatch FU", &cpu->renameDispatch);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[18] = "Rename Dispatch FU";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }    
    if (cpu->renameDispatch.has_insn)
    {
        /* Copy data from decode latch to execute latch*/
        //cpu->intFU = cpu->renameDispatch;
        //Add into Instruction Queue
        if(isIQFull(cpu) || isROBFull()){
           cpu->fetch_decode_from_next_cycle = TRUE; 
           return;
        }
        else if((cpu->renameDispatch.opcode == OPCODE_LOAD || cpu->renameDispatch.opcode == OPCODE_LDR ||
                 cpu->renameDispatch.opcode == OPCODE_STORE || cpu->renameDispatch.opcode == OPCODE_STR) && isLSQFull()){
           cpu->fetch_decode_from_next_cycle = TRUE; 
           return;                    
        }
        else{
            //new LSQ queue entry
            if(cpu->renameDispatch.opcode == OPCODE_LOAD || cpu->renameDispatch.opcode == OPCODE_LDR ||
                 cpu->renameDispatch.opcode == OPCODE_STORE || cpu->renameDispatch.opcode == OPCODE_STR){
                    cpu->newLSQEntry.lsqEntryEstablishedBit = 1;
                    cpu->newLSQEntry.instructionOpcode = cpu->renameDispatch.opcode;
                    cpu->newLSQEntry.pc = cpu->renameDispatch.pc;
                    cpu->newLSQEntry.rd = cpu->renameDispatch.rd;
                    cpu->newLSQEntry.rs1 = cpu->renameDispatch.rs1;
                    cpu->newLSQEntry.rs2 = cpu->renameDispatch.rs2;
                    cpu->newLSQEntry.rs3 = cpu->renameDispatch.rs3;
                    cpu->newLSQEntry.imm = cpu->renameDispatch.imm;
                    cpu->newLSQEntry.rs1Value = cpu->renameDispatch.rs1_value;
                    cpu->newLSQEntry.rs2Value = cpu->renameDispatch.rs2_value;
                    cpu->newLSQEntry.rs3Value = cpu->renameDispatch.rs3_value;
                    cpu->newLSQEntry.registerRenamed = cpu->renameDispatch.registerRenamed;
                    cpu->newLSQEntry.arcDest = cpu->renameDispatch.arcDest;
                    // cpu->newLSQEntry.applicationStage = cpu->
        switch (cpu->renameDispatch.opcode)
        {
            case OPCODE_LDR:
            case OPCODE_STORE:                        
            {
                //rs1 rs2
                if(sourceRegisterInInsQueue(cpu,cpu->newLSQEntry.rs1) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newLSQEntry.rs1) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newLSQEntry.rs1) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newLSQEntry.rs1)){
                    cpu->newLSQEntry.rs1ValidBit = 0;
                }else{
                    cpu->newLSQEntry.rs1ValidBit = 1;
                }
                if(sourceRegisterInInsQueue(cpu,cpu->newLSQEntry.rs2) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newLSQEntry.rs2) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newLSQEntry.rs2) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newLSQEntry.rs2)){
                    cpu->newLSQEntry.rs2ValidBit = 0;
                }else{
                    cpu->newLSQEntry.rs2ValidBit = 1;
                }   
                cpu->newLSQEntry.rs3ValidBit = 1;                           
                break;
            }
            case OPCODE_LOAD:                        
            {
                if(sourceRegisterInInsQueue(cpu,cpu->newLSQEntry.rs1) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newLSQEntry.rs1) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newLSQEntry.rs1) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newLSQEntry.rs1)){
                    cpu->newLSQEntry.rs1ValidBit = 0;
                }else{
                    cpu->newLSQEntry.rs1ValidBit = 1;
                }
                cpu->newLSQEntry.rs2ValidBit= 1;
                cpu->newLSQEntry.rs3ValidBit = 1;  
                break;
            }            
            case OPCODE_STR:
            {
                //rs1 rs2 rs3  
                if(sourceRegisterInInsQueue(cpu,cpu->newLSQEntry.rs1) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newLSQEntry.rs1) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newLSQEntry.rs1) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newLSQEntry.rs1)){
                    cpu->newLSQEntry.rs1ValidBit = 0;
                }else{
                    cpu->newLSQEntry.rs1ValidBit = 1;
                }
                if(sourceRegisterInInsQueue(cpu,cpu->newLSQEntry.rs2) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newLSQEntry.rs2) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newLSQEntry.rs2) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newLSQEntry.rs2)){
                    cpu->newLSQEntry.rs2ValidBit = 0;
                }else{
                    cpu->newLSQEntry.rs2ValidBit = 1;
                }
                if(sourceRegisterInInsQueue(cpu,cpu->newLSQEntry.rs3) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newLSQEntry.rs3) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newLSQEntry.rs3) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newLSQEntry.rs3)){
                    cpu->newLSQEntry.rs3ValidBit = 0;
                }else{
                    cpu->newLSQEntry.rs3ValidBit = 1;
                }                                                                                                                
                break;
            }                  
        }                                   
            //insertIQ(cpu,newInsQueueEntry); removed insert from here because we want iq entry to be available in next cycle
            if(cpu->forwardingBus1.tag == cpu->renameDispatch.rs1){
                cpu->newLSQEntry.rs1ValidBit = 1;
                cpu->newLSQEntry.rs1Value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->renameDispatch.rs1){
                cpu->newLSQEntry.rs1ValidBit = 1;
                cpu->newLSQEntry.rs1Value = cpu->forwardingBus2.data;
            }

            if(cpu->forwardingBus1.tag == cpu->renameDispatch.rs2){
                cpu->newLSQEntry.rs2ValidBit = 1;
                cpu->newLSQEntry.rs2Value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->renameDispatch.rs2){
                cpu->newLSQEntry.rs2ValidBit = 1;
                cpu->newLSQEntry.rs2Value = cpu->forwardingBus2.data;
            }

            if(cpu->forwardingBus1.tag == cpu->renameDispatch.rs3){
                cpu->newLSQEntry.rs3ValidBit = 1;
                cpu->newLSQEntry.rs3Value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->renameDispatch.rs3){
                cpu->newLSQEntry.rs3ValidBit = 1;
                cpu->newLSQEntry.rs3Value = cpu->forwardingBus2.data;
            }                        
                        
            insertLSQ(cpu);
            }else{
            cpu->newInsQueueEntry.validBit = 2;
            cpu->newInsQueueEntry.pc = cpu->renameDispatch.pc;
            cpu->newInsQueueEntry.opcode = cpu->renameDispatch.opcode;
            strcpy(cpu->newInsQueueEntry.opcode_str, cpu->renameDispatch.opcode_str);
            cpu->newInsQueueEntry.imm = cpu->renameDispatch.imm;
            cpu->newInsQueueEntry.rs1_tag = cpu->renameDispatch.rs1;
            cpu->newInsQueueEntry.rs1_value = cpu->renameDispatch.rs1_value;
            cpu->newInsQueueEntry.rs2_tag = cpu->renameDispatch.rs2;
            cpu->newInsQueueEntry.rs2_value = cpu->renameDispatch.rs2_value;
            cpu->newInsQueueEntry.rs3_tag = cpu->renameDispatch.rs3;
            cpu->newInsQueueEntry.rs3_value = cpu->renameDispatch.rs3_value; 
            cpu->newInsQueueEntry.rd = cpu->renameDispatch.rd;
            cpu->newInsQueueEntry.registerRenamed = cpu->renameDispatch.registerRenamed;
            cpu->newInsQueueEntry.arcDest = cpu->renameDispatch.arcDest;
            cpu->newInsQueueEntry.branchLinkAddr = cpu->renameDispatch.branchLinkAddr;
        switch (cpu->renameDispatch.opcode)
        {
            case OPCODE_SUB:
            case OPCODE_LDR:
            case OPCODE_STORE:                        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR:                                    
            case OPCODE_MUL:
            case OPCODE_CMP:            
            case OPCODE_ADD:
            {
                //rs1 rs2
                if(sourceRegisterInInsQueue(cpu,cpu->newInsQueueEntry.rs1_tag) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newInsQueueEntry.rs1_tag) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newInsQueueEntry.rs1_tag) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newInsQueueEntry.rs1_tag)){
                    cpu->newInsQueueEntry.rs1_valid = 0;
                }else{
                    cpu->newInsQueueEntry.rs1_valid = 1;
                }
                if(sourceRegisterInInsQueue(cpu,cpu->newInsQueueEntry.rs2_tag) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newInsQueueEntry.rs2_tag) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newInsQueueEntry.rs2_tag) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newInsQueueEntry.rs2_tag)){
                    cpu->newInsQueueEntry.rs2_valid = 0;
                    if(cpu->renameDispatch.pc == 4040){
                        // printf("4040 debug\n");
                        // printf("intfu rd %d\n",cpu->intFU.rd);
                        // printf("mulFU rd %d\n",cpu->mulFU.rd);
                        // printf("LogicalFU rd %d\n",cpu->logicalFU.rd);
                    }
                }else{
                    cpu->newInsQueueEntry.rs2_valid = 1;
                }     
                cpu->newInsQueueEntry.rs3_valid = 1;                           
                break;
            }
            case OPCODE_SUBL:
            case OPCODE_JUMP:
            case OPCODE_LOAD:                        
            case OPCODE_ADDL:
            {
                if(sourceRegisterInInsQueue(cpu,cpu->newInsQueueEntry.rs1_tag) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newInsQueueEntry.rs1_tag) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newInsQueueEntry.rs1_tag) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newInsQueueEntry.rs1_tag)){
                    cpu->newInsQueueEntry.rs1_valid = 0;
                }else{
                    cpu->newInsQueueEntry.rs1_valid = 1;
                }
                cpu->newInsQueueEntry.rs2_valid = 1;
                cpu->newInsQueueEntry.rs3_valid = 1;  
                break;
            }            
            case OPCODE_STR:
            {
                //rs1 rs2 rs3  
                if(sourceRegisterInInsQueue(cpu,cpu->newInsQueueEntry.rs1_tag) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newInsQueueEntry.rs1_tag) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newInsQueueEntry.rs1_tag) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newInsQueueEntry.rs1_tag)){
                    cpu->newInsQueueEntry.rs1_valid = 0;
                    //printf("validity unset for rs1_valid");
                }else{
                    cpu->newInsQueueEntry.rs1_valid = 1;
                }
                if(sourceRegisterInInsQueue(cpu,cpu->newInsQueueEntry.rs2_tag) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newInsQueueEntry.rs2_tag) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newInsQueueEntry.rs2_tag) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newInsQueueEntry.rs2_tag)){
                    cpu->newInsQueueEntry.rs2_valid = 0;
                    //printf("validity unset for rs1_valid");
                }else{
                    cpu->newInsQueueEntry.rs2_valid = 1;
                }
                if(sourceRegisterInInsQueue(cpu,cpu->newInsQueueEntry.rs2_tag) || (cpu->intFU.has_insn == 1 && cpu->intFU.rd == cpu->newInsQueueEntry.rs2_tag) ||
                  (cpu->mulFU.has_insn == 1 && cpu->mulFU.rd == cpu->newInsQueueEntry.rs2_tag) || (cpu->logicalFU.has_insn == 1 && cpu->logicalFU.rd == cpu->newInsQueueEntry.rs2_tag)){
                    cpu->newInsQueueEntry.rs3_valid = 0;
                }else{
                    cpu->newInsQueueEntry.rs3_valid = 1;
                }                                                                                                
                break;
            }       
            case OPCODE_HALT:     
            case OPCODE_MOVC:
            {
                /* MOVC doesn't have register operands */
                
                cpu->newInsQueueEntry.rs1_valid = 1;
                cpu->newInsQueueEntry.rs2_valid = 1;
                cpu->newInsQueueEntry.rs3_valid = 1;
                break;
            }           
        }                                   
            //insertIQ(cpu,newInsQueueEntry); removed insert from here because we want iq entry to be available in next cycle
            if(cpu->forwardingBus1.tag == cpu->renameDispatch.rs1){
                cpu->newInsQueueEntry.rs1_valid = 1;
                cpu->newInsQueueEntry.rs1_value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->renameDispatch.rs1){
                cpu->newInsQueueEntry.rs1_valid = 1;
                cpu->newInsQueueEntry.rs1_value = cpu->forwardingBus2.data;
            }
            if(cpu->forwardingBus1.tag == cpu->renameDispatch.rs2){
                cpu->newInsQueueEntry.rs2_valid = 1;
                cpu->newInsQueueEntry.rs2_value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->renameDispatch.rs2){
                cpu->newInsQueueEntry.rs2_valid = 1;
                cpu->newInsQueueEntry.rs2_value = cpu->forwardingBus2.data;
            }
            if(cpu->forwardingBus1.tag == cpu->renameDispatch.rs3){
                cpu->newInsQueueEntry.rs3_valid = 1;
                cpu->newInsQueueEntry.rs3_value = cpu->forwardingBus1.data;
            }else if(cpu->forwardingBus2.tag == cpu->renameDispatch.rs3){
                cpu->newInsQueueEntry.rs3_valid = 1;
                cpu->newInsQueueEntry.rs3_value = cpu->forwardingBus2.data;
            }                        
            newInstructionEntryindex = insertIQ(cpu);                
            }
            //new Instruction queue entry
            if(cpu->renameDispatch.opcode != OPCODE_JUMP){
                insertROB(cpu);
            }
            //printf("insert ho rha hai");
            //printf("size of ins queue %d", sizeIQ(cpu));
            //printf("zero value %d",cpu->insQueue[0].imm);
            cpu->renameDispatch.has_insn = FALSE;
        }
         //kaam!

        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("Rename/Dispatch", &cpu->renameDispatch);
        // }
    }
    // if(cpu->renameDispatch.pc == 4052){
    //     printf("4052 rename me rs1 value %d\n",cpu->renameDispatch.rs1_value);
    // }

}

/*
 * Execute Stage of APEX Pipeline
 *
 * Note: You are free to edit this function according to your implementation
 */
static void
APEX_intFU(APEX_CPU *cpu)
{
    //printf("Int FU has ins field %d \n",cpu->intFU.has_insn);
    int deleteIndex;
    if(!cpu->intFU.has_insn){
        for (int i = 0; i < Instruction_QUEUE_SIZE; i++)
        {
            if(cpu->insQueue[i].validBit == 1 && cpu->insQueue[i].rs1_valid == 1 &&
       cpu->insQueue[i].rs2_valid == 1 && cpu->insQueue[i].rs3_valid == 1){
        if(cpu->insQueue[i].opcode == OPCODE_ADD || cpu->insQueue[i].opcode == OPCODE_ADDL || cpu->insQueue[i].opcode == OPCODE_SUB ||
           cpu->insQueue[i].opcode == OPCODE_SUBL || cpu->insQueue[i].opcode == OPCODE_CMP || cpu->insQueue[i].opcode == OPCODE_LOAD ||
           cpu->insQueue[i].opcode == OPCODE_LDR || cpu->insQueue[i].opcode == OPCODE_STORE || cpu->insQueue[i].opcode == OPCODE_STR ||
           cpu->insQueue[i].opcode == OPCODE_BZ || cpu->insQueue[i].opcode == OPCODE_BNZ || cpu->insQueue[i].opcode == OPCODE_MOVC || 
           cpu->insQueue[i].opcode == OPCODE_HALT || cpu->insQueue[i].opcode == OPCODE_JUMP || cpu->insQueue[i].opcode == OPCODE_NOP){
            cpu->intFU.has_insn = 1;
            strcpy(cpu->intFU.opcode_str, cpu->insQueue[i].opcode_str);
            cpu->intFU.opcode = cpu->insQueue[i].opcode;
            cpu->intFU.rs1 = cpu->insQueue[i].rs1_tag;
            cpu->intFU.rs2 = cpu->insQueue[i].rs2_tag;
            cpu->intFU.rs3 = cpu->insQueue[i].rs3_tag;
            cpu->intFU.rd = cpu->insQueue[i].rd;
            cpu->intFU.imm = cpu->insQueue[i].imm;
            cpu->intFU.rs1_value = cpu->insQueue[i].rs1_value;
            cpu->intFU.rs2_value = cpu->insQueue[i].rs2_value;
            cpu->intFU.rs3_value = cpu->insQueue[i].rs3_value;
            cpu->intFU.pc = cpu->insQueue[i].pc;
            cpu->intFU.registerRenamed = cpu->insQueue[i].registerRenamed;
            cpu->intFU.branchLinkAddr=cpu->insQueue[i].branchLinkAddr;
            deleteIndex = i;
            break;
            }
        }
    }
    }
   //printf("New Instruction Entry index %d",cpu->insQueue[newInstructionEntryindex].rd);
   for(int i=0;i<sizeIQ(cpu);i++){
    if(cpu->insQueue[i].validBit == 2 && (cpu->insQueue[i].opcode == OPCODE_ADD || cpu->insQueue[i].opcode == OPCODE_ADDL || cpu->insQueue[i].opcode == OPCODE_SUB ||
        cpu->insQueue[i].opcode == OPCODE_SUBL || cpu->insQueue[i].opcode == OPCODE_CMP || cpu->insQueue[i].opcode == OPCODE_LOAD ||
        cpu->insQueue[i].opcode == OPCODE_LDR || cpu->insQueue[i].opcode == OPCODE_STORE || cpu->insQueue[i].opcode == OPCODE_STR ||
         cpu->insQueue[i].opcode == OPCODE_BZ || cpu->insQueue[i].opcode == OPCODE_BNZ || cpu->insQueue[i].opcode == OPCODE_MOVC || cpu->insQueue[i].opcode == OPCODE_HALT ||
         cpu->insQueue[i].opcode == OPCODE_JUMP || cpu->insQueue[i].opcode == OPCODE_NOP)){
                cpu->insQueue[i].validBit = 1;
                    //printf("is valid");
    }   
   }
    if (cpu->intFU.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("Int FU 1", &cpu->intFU);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[9] = "Int FU";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
        //cpu->intFU.rd = 123; //double
    }    
    if (cpu->intFU.has_insn)
    {
        int insQueueEntryForwarded = -1;
        int registerForwarded = -1;
        if(isWQEmpty() && cpu->forwardingBus1.tagLock!=1){
            cpu->intFU.allocatedForwardingBus = 1;
            cpu->forwardingBus1.tagLock=1;
            cpu->forwardingBus1.tag = cpu->intFU.rd;
        }
        else if(isWQEmpty() && cpu->forwardingBus2.tagLock!=1){
            cpu->intFU.allocatedForwardingBus = 2;
            cpu->forwardingBus2.tagLock=1;
            cpu->forwardingBus2.tag = cpu->intFU.rd;        
        }else{
            cpu->intFU.allocatedForwardingBus = 3;
            cpu->intFU.noBusAllocated = 1;
            insertWQ(cpu,1);
        }

        removeDataIQ(cpu,deleteIndex);
        //post the tag into instruction queue
        for (int i = 0; i < iQCount; i++)
        {
            if(cpu->intFU.allocatedForwardingBus == 1){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 1;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 2;
                    //break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 3;
                    //break;                    
                }                                
            }else if(cpu->intFU.allocatedForwardingBus == 2){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 1;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 2;
                    //break;                    
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 3;
                    //break;                    
                }                                
            }
        }    


        //post the tag to rename dispatch stage
            // if(cpu->intFU.allocatedForwardingBus == 1){
            //     if(cpu->newInsQueueEntry.rs1_valid == 0 && cpu->newInsQueueEntry.rs1_tag == cpu->forwardingBus1.tag){
            //         cpu->newInsQueueEntry.rs1_valid = 1;
            //         insQueueEntryForwarded = 1000;
            //         registerForwarded = 1;                   
            //     }
            //     if(cpu->newInsQueueEntry.rs2_valid == 0 && cpu->newInsQueueEntry.rs2_tag == cpu->forwardingBus1.tag){
            //         cpu->newInsQueueEntry.rs2_valid = 1;
            //         insQueueEntryForwarded = 1000;
            //         registerForwarded = 2;               
            //     }                
            //     if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
            //         cpu->insQueue[i].rs3_valid = 1;
            //         insQueueEntryForwarded = 1000;
            //         registerForwarded = 3;                  
            //     }                                
            // }else if(cpu->intFU.allocatedForwardingBus == 2){
            //     if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
            //         cpu->insQueue[i].rs1_valid = 1;
            //         insQueueEntryForwarded = 1000;
            //         registerForwarded = 1;                
            //     }
            //     if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
            //         cpu->insQueue[i].rs2_valid = 1;
            //         insQueueEntryForwarded = 1000;
            //         registerForwarded = 2;                   
            //     }                
            //     if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
            //         cpu->insQueue[i].rs3_valid = 1;
            //         insQueueEntryForwarded = 1000;
            //         registerForwarded = 3;                   
            //     }                                
            // }            
        /* Execute logic based on instruction type */
        switch (cpu->intFU.opcode)
        {
            case OPCODE_ADD:
            {
                cpu->intFU.result_buffer
                    = cpu->intFU.rs1_value + cpu->intFU.rs2_value;

                /* Set the zero flag based on the result buffer */
                if (cpu->intFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->intFU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->intFU.rd] = 2;
                }
                break;
            }
            case OPCODE_ADDL:
            {
                // if(cpu->intFU.pc == 4052){
                //     printf("Debug 4052 %d %d\n",cpu->intFU.rs1_value,cpu->intFU.imm);
                // }
                cpu->intFU.result_buffer
                    = cpu->intFU.rs1_value + cpu->intFU.imm;

                /* Set the zero flag based on the result buffer */
                if (cpu->intFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->intFU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->intFU.rd] = 2;
                }
                break;
            }            
            case OPCODE_SUB:
            {
                cpu->intFU.result_buffer
                    = cpu->intFU.rs1_value - cpu->intFU.rs2_value;
                    //printf("In Execute stage %d",cpu->execute.rs1_value);
                    //printf("In Execute stage %d",cpu->execute.rs2_value);

                /* Set the zero flag based on the result buffer */
                if (cpu->intFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->intFU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->intFU.rd] = 2;
                }
                break;
            }     
            case OPCODE_SUBL:
            {
                cpu->intFU.result_buffer
                    = cpu->intFU.rs1_value - cpu->intFU.imm;

                /* Set the zero flag based on the result buffer */
                if (cpu->intFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->intFU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->intFU.rd] = 2;
                }
                break;
            }       
            case OPCODE_CMP:
            {
                // cpu->execute.result_buffer
                //     = cpu->execute.rs1_value + cpu->execute.rs2_value;

                /* Set the zero flag based on the result buffer */
                if(cpu->intFU.pc == 4056){
                    printf("4056 debug %d %d %d\n",cpu->intFU.rs1_value,cpu->intFU.rs2_value,cpu->intFU.rd);
                }
                if (cpu->intFU.rs1_value == cpu->intFU.rs2_value)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->intFU.rd] = 1;
                    printf("CMP here \n");
                } 
                else 
                {
                    printf("CMP there \n");
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->intFU.rd] = 2;
                }
                break;
            }                                    
            case OPCODE_LOAD:
            {
                cpu->intFU.memory_address
                    = cpu->intFU.rs1_value + cpu->intFU.imm;
                //printf("*********MY text EXECUTE%d\n",cpu->execute.memory_address);                    
                break;
            }
            case OPCODE_LDR:
            {
                cpu->intFU.memory_address
                    = cpu->intFU.rs1_value + cpu->intFU.rs2_value;
                //printf("*********MY text EXECUTE%d\n",cpu->execute.memory_address);                    
                break;
            }            
            case OPCODE_STORE:
            {
                cpu->intFU.memory_address
                    = cpu->intFU.rs2_value + cpu->intFU.imm;
                break;
            }
            case OPCODE_STR:
            {
                cpu->intFU.memory_address
                    = cpu->intFU.rs2_value + cpu->intFU.rs1_value;
                break;
            }            
            case OPCODE_BZ:
            {
                if(cpu->intFU.pc == 4044){
                    printf("4044 debug %d %d\n",cpu->intFU.branchLinkAddr,cpu->ccFlag[cpu->intFU.branchLinkAddr]);
                    printf("rename me kya chal rha hai %d  %d\n",cpu->renameDispatch.rd,cpu->decode.rd);
                    
                }                
                printf("Flag value BZ %d",cpu->ccFlag[cpu->intFU.rd]);
                if (cpu->ccFlag[cpu->intFU.branchLinkAddr] == 1)
                {
                    /* Calculate new PC, and send it to fetch unit */
                    cpu->pc = cpu->intFU.pc + cpu->intFU.imm;

                    cpu->ccFlag[cpu->intFU.branchLinkAddr] = 0;
                    
                    /* Since we are using reverse callbacks for pipeline stages, 
                     * this will prevent the new instruction from being fetched in the current cycle*/
                    cpu->fetch_from_next_cycle = TRUE;

                    /* Flush previous stages */
                    if(cpu->decode.has_insn){
                    cpu->decode.has_insn = FALSE;
                     cpu->renameTable[cpu->decode.arcDest] = cpu->decode.registerRenamed;
                     insert(cpu,cpu->decode.rd);
                    }
                    if(cpu->renameDispatch.has_insn){
                    cpu->renameDispatch.has_insn = FALSE;
                    cpu->renameTable[cpu->renameDispatch.arcDest] = cpu->renameDispatch.registerRenamed;
                    insert(cpu,cpu->renameDispatch.rd);
                    }
                    
                    if((cpu->intFU.pc-4) >=4000){
                        int addressToBeSearched = cpu->intFU.pc-4;
                        //instruction queue search
                        for (int i = 0; i < rearIQ; i++){
                            if(cpu->insQueue[i].pc == addressToBeSearched){
                                cpu->renameTable[cpu->insQueue[i].arcDest] = cpu->insQueue[i].registerRenamed;
                                insert(cpu,cpu->insQueue[i].rd);                                
                            }
                        }      
                        //LSQ to be searched
                        if(lsqTail >= lsqHead)
    {
     
        // Loop to print elements from
        // front to rear.
        for(int i = lsqHead; i < lsqTail; i++)
        {            
            cpu->renameTable[cpu->lsq[i].arcDest] = cpu->lsq[i].registerRenamed;
            insert(cpu,cpu->lsq[i].rd);                
        }
    }
 
    // If rear crossed the max index and
    // indexing has started in loop
    else
    {      
        // Loop for printing elements from
        // front to max size or last index
        for(int i = lsqHead; i < LSQ_SIZE; i++)
        {             
            cpu->renameTable[cpu->lsq[i].arcDest] = cpu->lsq[i].registerRenamed;
            insert(cpu,cpu->lsq[i].rd);                   
        }
 
        // Loop for printing elements from
        // 0th index till rear position
        for(int i = 0; i < lsqTail; i++)
        {        
            cpu->renameTable[cpu->lsq[i].arcDest] = cpu->lsq[i].registerRenamed;
            insert(cpu,cpu->lsq[i].rd);               
        }
    }                                          
                    }
                    //cpu->renameTable[cpu->renameDispatch.arcDest] = cpu->renameDispatch.registerRenamed;
                    /* Make sure fetch stage is enabled to start fetching from new PC */
                    cpu->fetch.has_insn = TRUE;
                    //printf("ROB head BZ before %d",cpu->rob[robHead].pc);
                    //dequeueROB(cpu);
                    //dummy
                    
                    int dumyTail = (robTail - 1) % ROB_SIZE;
                    if(cpu->rob[dumyTail].instructionOpcode == OPCODE_LOAD || cpu->rob[dumyTail].instructionOpcode == OPCODE_LDR
                     || cpu->rob[dumyTail].instructionOpcode == OPCODE_STORE || cpu->rob[dumyTail].instructionOpcode == OPCODE_STR){
                        removeTailLSQ(cpu);
                    }
                    removeTailROB(cpu);
                    //printf("ROB head BZ %d",cpu->rob[robHead].pc);
                }
                break;
            }

            case OPCODE_JUMP:
            {
                    /* Calculate new PC, and send it to fetch unit */
                    cpu->pc = cpu->intFU.rs1_value + cpu->intFU.imm;
                    printf("rs1 value in jump %d\n",cpu->intFU.rs1_value);
                    /* Since we are using reverse callbacks for pipeline stages, 
                     * this will prevent the new instruction from being fetched in the current cycle*/
                    // cpu->fetch_from_next_cycle = TRUE;

                    /* Flush previous stages */
                    // cpu->decode.has_insn = FALSE;

                    /* Make sure fetch stage is enabled to start fetching from new PC */
                    // cpu->fetch.has_insn = TRUE;
                break;
            }            

            case OPCODE_BNZ:
            {
                printf("Flag value BNZ %d",cpu->ccFlag[cpu->intFU.branchLinkAddr]);
                if (cpu->ccFlag[cpu->intFU.branchLinkAddr] == 2)
                {
                    //printf("Branch Lia hai ");
                    /* Calculate new PC, and send it to fetch unit */
                    cpu->pc = cpu->intFU.pc + cpu->intFU.imm;
                    cpu->ccFlag[cpu->intFU.branchLinkAddr] = 0;
                    /* Since we are using reverse callbacks for pipeline stages, 
                     * this will prevent the new instruction from being fetched in the current cycle*/
                    cpu->fetch_from_next_cycle = TRUE;

                    /* Flush previous stages */
                    cpu->decode.has_insn = FALSE;

                    cpu->renameDispatch.has_insn = FALSE;

                    /* Make sure fetch stage is enabled to start fetching from new PC */
                    cpu->fetch.has_insn = TRUE;
                     //printf("ROB head BNZ before %d",cpu->rob[robHead].pc);
                    // dequeueROB(cpu);
                
                     printf("ROB tail BNZ %d",cpu->rob[robTail].pc);
                    removeTailROB(cpu);
                }
                break;
            }

            case OPCODE_MOVC: 
            {
                cpu->intFU.result_buffer = cpu->intFU.imm;

                /* Set the zero flag based on the result buffer */
                if (cpu->intFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->intFU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                   cpu->ccFlag[cpu->intFU.rd] = 2;
                    
                }
                break;
            }
        }

        /* Copy data from execute latch to memory latch*/
        // cpu->writeback = cpu->intFU;
        // cpu->intFU.has_insn = FALSE;
        if(cpu->intFU.opcode == OPCODE_BNZ || cpu->intFU.opcode == OPCODE_CMP){
            //printf("Kaunsa forwarding bus allocate hua inko %d\n",cpu->intFU.allocatedForwardingBus);
        }
    if(cpu->intFU.allocatedForwardingBus == 1){
        cpu->forwardingBus1.data = cpu->intFU.result_buffer;
        cpu->forwardingBus1.tag = cpu->intFU.rd;
        cpu->forwardingBus1.applicationStage = cpu->intFU;        
        cpu->intFU.has_insn = FALSE;
    }
    else if(cpu->intFU.allocatedForwardingBus == 2){
        cpu->forwardingBus2.data = cpu->intFU.result_buffer;
        cpu->forwardingBus2.tag = cpu->intFU.rd;
        cpu->forwardingBus2.applicationStage = cpu->intFU;
        cpu->intFU.has_insn = FALSE;        
    }

    if(insQueueEntryForwarded != -1){
        if(registerForwarded == 1){
            cpu->insQueue[insQueueEntryForwarded].rs1_value = cpu->intFU.result_buffer; 
        } else if(registerForwarded == 2){
            cpu->insQueue[insQueueEntryForwarded].rs2_value = cpu->intFU.result_buffer; 
        } else if(registerForwarded == 3){
            cpu->insQueue[insQueueEntryForwarded].rs3_value = cpu->intFU.result_buffer; 
        }
    }

        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("Integer FU", &cpu->intFU);
        // }
    }
}

static void
APEX_mulFU1(APEX_CPU *cpu){  
    // if(cpu->pc == 4032){
    //                 printf("4032 debug rename me kya chal rha hai %d  %d\n",cpu->renameDispatch.rd,cpu->decode.rd);
    // }  
    if(cpu->mulFU.has_insn){
        return;
    }
    int deleteIndex;
    if(!cpu->mul1FU.has_insn){
   for (int i = 0; i < Instruction_QUEUE_SIZE; i++)
   {
    if(cpu->insQueue[i].opcode == OPCODE_MUL){
        //printf("Mul validity %d\n",cpu->insQueue[i].validBit);
        //printf("Mul validity %d %d %d\n",cpu->insQueue[i].rs1_valid,cpu->insQueue[i].rs2_valid,cpu->insQueue[i].rs3_valid);
    }
    if(cpu->insQueue[i].validBit == 1 && cpu->insQueue[i].rs1_valid == 1 &&
       cpu->insQueue[i].rs2_valid == 1 && cpu->insQueue[i].rs3_valid == 1){
        if(cpu->insQueue[i].opcode == OPCODE_MUL){
            cpu->mul1FU.has_insn = 1;
            strcpy(cpu->mul1FU.opcode_str, cpu->insQueue[i].opcode_str);
            cpu->mul1FU.opcode = cpu->insQueue[i].opcode;
            cpu->mul1FU.rs1 = cpu->insQueue[i].rs1_tag;
            cpu->mul1FU.rs2 = cpu->insQueue[i].rs2_tag;
            cpu->mul1FU.rs3 = cpu->insQueue[i].rs3_tag;
            cpu->mul1FU.rd = cpu->insQueue[i].rd;
            cpu->mul1FU.imm = cpu->insQueue[i].imm;
            cpu->mul1FU.rs1_value = cpu->insQueue[i].rs1_value;
            cpu->mul1FU.rs2_value = cpu->insQueue[i].rs2_value;
            cpu->mul1FU.rs3_value = cpu->insQueue[i].rs3_value;
            cpu->mul1FU.pc = cpu->insQueue[i].pc;
            cpu->mul1FU.registerRenamed = cpu->insQueue[i].registerRenamed;
            deleteIndex = i;
            //printf("delete index %d",deleteIndex);
            break;
        }
    }
   }         
    }
   for(int i=0;i<sizeIQ(cpu);i++){
    if(cpu->insQueue[i].validBit == 2 && cpu->insQueue[i].opcode == OPCODE_MUL){
                cpu->insQueue[i].validBit = 1;
                    //printf("is valid");
    }   
   }
    if (cpu->mul1FU.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("MUl FU 1", &cpu->mul1FU);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[9] = "Mul FU 1";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }   
   if(cpu->mul1FU.has_insn){
    removeDataIQ(cpu,deleteIndex);
    cpu->mul2FU = cpu->mul1FU;
    cpu->mul1FU.has_insn = FALSE;
        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("MUl FU 1", &cpu->mul1FU);
        // }    
        switch (cpu->mul1FU.opcode)
        {
            case OPCODE_MUL:
            {
                cpu->mul1FU.result_buffer
                    = cpu->mul1FU.rs1_value * cpu->mul1FU.rs2_value;
                //printf("\nMul value %d %d %d\n",cpu->mulFU.result_buffer,cpu->mulFU.rs1_value,cpu->mulFU.rs2_value);    

                /* Set the zero flag based on the result buffer */
                if (cpu->mul1FU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->mul1FU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->mul1FU.rd] = 2;
                }
                break;
            }            
        }        
   }    
}
static void
APEX_mulFU2(APEX_CPU *cpu){
    if (cpu->mul2FU.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("MUl FU 2", &cpu->mul2FU);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[9] = "Mul FU 2";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }        
    if(cpu->mulFU.has_insn){
        return;
    }    
   if(cpu->mul2FU.has_insn){
    cpu->mul3FU = cpu->mul2FU;
    cpu->mul2FU.has_insn = FALSE;
        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("MUl FU 2", &cpu->mul2FU);
        // }    
   }
}
static void
APEX_mulFU3(APEX_CPU *cpu){
    if (cpu->mul3FU.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("MUl FU 3", &cpu->mul3FU);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[9] = "Mul FU 3";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }    
    if(cpu->mulFU.has_insn){
        return;
    }    
   if(cpu->mul3FU.has_insn){
    cpu->mulFU = cpu->mul3FU;
    cpu->mul3FU.has_insn = FALSE;
        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("MUl FU 3", &cpu->mul3FU);
        // }    
   }    
}


//MulFU
static void
APEX_mulFU(APEX_CPU *cpu)
{
    if (cpu->mulFU.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("Multiply FU", &cpu->mulFU);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[11] = "Multiply FU";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }    
//     int deleteIndex;
//     if(!cpu->mulFU.has_insn){
//    for (int i = 0; i < Instruction_QUEUE_SIZE; i++)
//    {
//     if(cpu->insQueue[i].opcode == OPCODE_MUL){
//         //printf("Mul validity %d\n",cpu->insQueue[i].validBit);
//         //printf("Mul validity %d %d %d\n",cpu->insQueue[i].rs1_valid,cpu->insQueue[i].rs2_valid,cpu->insQueue[i].rs3_valid);
//     }
//     if(cpu->insQueue[i].validBit == 1 && cpu->insQueue[i].rs1_valid == 1 &&
//        cpu->insQueue[i].rs2_valid == 1 && cpu->insQueue[i].rs3_valid == 1){
//         if(cpu->insQueue[i].opcode == OPCODE_MUL){
//             cpu->mulFU.has_insn = 1;
//             strcpy(cpu->mulFU.opcode_str, cpu->insQueue[i].opcode_str);
//             cpu->mulFU.opcode = cpu->insQueue[i].opcode;
//             cpu->mulFU.rs1 = cpu->insQueue[i].rs1_tag;
//             cpu->mulFU.rs2 = cpu->insQueue[i].rs2_tag;
//             cpu->mulFU.rs3 = cpu->insQueue[i].rs3_tag;
//             cpu->mulFU.rd = cpu->insQueue[i].rd;
//             cpu->mulFU.imm = cpu->insQueue[i].imm;
//             cpu->mulFU.rs1_value = cpu->insQueue[i].rs1_value;
//             cpu->mulFU.rs2_value = cpu->insQueue[i].rs2_value;
//             cpu->mulFU.rs3_value = cpu->insQueue[i].rs3_value;
//             cpu->mulFU.pc = cpu->insQueue[i].pc;
//             cpu->mulFU.registerRenamed = cpu->insQueue[i].registerRenamed;
//             deleteIndex = i;
//             //printf("delete index %d",deleteIndex);
//             break;
//         }
//     }
//    }         
//     }
//    for(int i=0;i<sizeIQ(cpu);i++){
//     if(cpu->insQueue[i].validBit == 2 && cpu->insQueue[i].opcode == OPCODE_MUL){
//                 cpu->insQueue[i].validBit = 1;
//                     //printf("is valid");
//     }   
//    }     
    if (cpu->mulFU.has_insn)
    {
        int insQueueEntryForwarded = -1;
        int registerForwarded = -1;
        if(isWQEmpty() && cpu->forwardingBus1.tagLock!=1){
            cpu->mulFU.allocatedForwardingBus = 1;
            cpu->forwardingBus1.tagLock=1;
            cpu->forwardingBus1.tag = cpu->mulFU.rd;
        }
        else if(isWQEmpty() && cpu->forwardingBus2.tagLock!=1){
            cpu->mulFU.allocatedForwardingBus = 2;
            cpu->forwardingBus2.tagLock=1;
            cpu->forwardingBus2.tag = cpu->mulFU.rd;        
        }else{
            cpu->mulFU.allocatedForwardingBus = 3;
            cpu->mulFU.noBusAllocated = 1;
            insertWQ(cpu,3);
        }        
        //printf("what are we deleting here %d",cpu->insQueue[deleteIndex].opcode);
        //removeDataIQ(cpu,deleteIndex);
        //printf("size after deleting mul %d",sizeIQ(cpu));
        /* Execute logic based on instruction type */
        for (int i = 0; i < iQCount; i++)
        {
            if(cpu->mulFU.allocatedForwardingBus == 1){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 1;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 2;
                    //break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 3;
                    //break;                    
                }                                
            }else if(cpu->mulFU.allocatedForwardingBus == 2){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 1;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 2;
                    //break;                    
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 3;
                    //break;                    
                }                                
            }
        }        
        // switch (cpu->mulFU.opcode)
        // {
        //     case OPCODE_MUL:
        //     {
        //         cpu->mulFU.result_buffer
        //             = cpu->mulFU.rs1_value * cpu->mulFU.rs2_value;
        //         //printf("\nMul value %d %d %d\n",cpu->mulFU.result_buffer,cpu->mulFU.rs1_value,cpu->mulFU.rs2_value);    

        //         /* Set the zero flag based on the result buffer */
        //         if (cpu->mulFU.result_buffer == 0)
        //         {
        //             cpu->zero_flag = TRUE;
        //         } 
        //         else 
        //         {
        //             cpu->zero_flag = FALSE;
        //         }
        //         break;
        //     }            
        // }

        /* Copy data from execute latch to memory latch*/
        // cpu->writeback = cpu->mulFU;
        // cpu->mulFU.has_insn = FALSE;
    if(cpu->mulFU.allocatedForwardingBus == 1){
        cpu->forwardingBus1.data = cpu->mulFU.result_buffer;
        cpu->forwardingBus1.tag = cpu->mulFU.rd;
        cpu->forwardingBus1.applicationStage = cpu->mulFU;        
        cpu->mulFU.has_insn = FALSE;
    }
    else if(cpu->mulFU.allocatedForwardingBus == 2){
        cpu->forwardingBus2.data = cpu->mulFU.result_buffer;
        cpu->forwardingBus2.tag = cpu->mulFU.rd;
        cpu->forwardingBus2.applicationStage = cpu->mulFU;
        cpu->mulFU.has_insn = FALSE;        
    }

    if(insQueueEntryForwarded != -1){
        if(registerForwarded == 1){
            cpu->insQueue[insQueueEntryForwarded].rs1_value = cpu->mulFU.result_buffer; 
        } else if(registerForwarded == 2){
            cpu->insQueue[insQueueEntryForwarded].rs2_value = cpu->mulFU.result_buffer; 
        } else if(registerForwarded == 3){
            cpu->insQueue[insQueueEntryForwarded].rs3_value = cpu->mulFU.result_buffer; 
        }
    }        

        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("Multiply FU", &cpu->mulFU);
        // }
    }
}



//Logical FU
static void
APEX_logicalFU(APEX_CPU *cpu)
{    
    int deleteIndex;
    if(!cpu->logicalFU.has_insn){
   for (int i = 0; i < Instruction_QUEUE_SIZE; i++)
   {
    if(cpu->insQueue[i].validBit == 1 && cpu->insQueue[i].rs1_valid == 1 &&
       cpu->insQueue[i].rs2_valid == 1 && cpu->insQueue[i].rs3_valid == 1){
        if(cpu->insQueue[i].opcode == OPCODE_AND || cpu->insQueue[i].opcode == OPCODE_OR || cpu->insQueue[i].opcode == OPCODE_XOR){
            cpu->logicalFU.has_insn = 1;
            strcpy(cpu->logicalFU.opcode_str, cpu->insQueue[i].opcode_str);
            cpu->logicalFU.opcode = cpu->insQueue[i].opcode;
            cpu->logicalFU.rs1 = cpu->insQueue[i].rs1_tag;
            cpu->logicalFU.rs2 = cpu->insQueue[i].rs2_tag;
            cpu->logicalFU.rs3 = cpu->insQueue[i].rs3_tag;
            cpu->logicalFU.rd = cpu->insQueue[i].rd;
            cpu->logicalFU.imm = cpu->insQueue[i].imm;
            cpu->logicalFU.rs1_value = cpu->insQueue[i].rs1_value;
            cpu->logicalFU.rs2_value = cpu->insQueue[i].rs2_value;
            cpu->logicalFU.rs3_value = cpu->insQueue[i].rs3_value;
            cpu->logicalFU.pc = cpu->insQueue[i].pc;
            deleteIndex = i;
            break;
        }
    }
   }
    }
   for(int i=0;i<sizeIQ(cpu);i++){
    if(cpu->insQueue[i].validBit == 2 && (cpu->insQueue[i].opcode == OPCODE_AND || cpu->insQueue[i].opcode == OPCODE_OR ||
                         cpu->insQueue[i].opcode == OPCODE_XOR)){
                cpu->insQueue[i].validBit = 1;
                    //printf("is valid");
    }   
   }
   
    if (cpu->logicalFU.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("Logical FU", &cpu->logicalFU);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[10] = "Logical FU";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }
    if (cpu->logicalFU.has_insn)
    {
        int insQueueEntryForwarded = -1;
        int registerForwarded = -1;
        if(isWQEmpty() && cpu->forwardingBus1.tagLock!=1){
            cpu->logicalFU.allocatedForwardingBus = 1;
            cpu->forwardingBus1.tagLock=1;
            cpu->forwardingBus1.tag = cpu->logicalFU.rd;
        }
        else if(isWQEmpty() && cpu->forwardingBus2.tagLock!=1){
            cpu->logicalFU.allocatedForwardingBus = 2;
            cpu->forwardingBus2.tagLock=1;
            cpu->forwardingBus2.tag = cpu->logicalFU.rd;        
        }else{
            cpu->logicalFU.allocatedForwardingBus = 3;
            cpu->logicalFU.noBusAllocated = 1;
            insertWQ(cpu,4);
        }        
        removeDataIQ(cpu,deleteIndex);
        /* Execute logic based on instruction type */
        for (int i = 0; i < iQCount; i++)
        {
            if(cpu->logicalFU.allocatedForwardingBus == 1){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 1;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 2;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 3;
                    break;                    
                }                                
            }else if(cpu->logicalFU.allocatedForwardingBus == 2){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 1;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 2;
                    break;                    
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    insQueueEntryForwarded = i;
                    registerForwarded = 3;
                    break;                    
                }                                
            }
        }        
        switch (cpu->logicalFU.opcode)
        {
            case OPCODE_AND:
            {
                cpu->logicalFU.result_buffer
                    = cpu->logicalFU.rs1_value & cpu->logicalFU.rs2_value;

                /* Set the zero flag based on the result buffer */
                if (cpu->logicalFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->logicalFU.rd] = 1;
                } 
                else 
                {
                    cpu->ccFlag[cpu->logicalFU.rd] = 2;
                    cpu->zero_flag = FALSE;
                }
                break;
            }
            case OPCODE_OR:
            {
                cpu->logicalFU.result_buffer
                    = cpu->logicalFU.rs1_value | cpu->logicalFU.rs2_value;

                /* Set the zero flag based on the result buffer */
                if (cpu->logicalFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->logicalFU.rd] = 1;
                } 
                else 
                {
                    cpu->ccFlag[cpu->logicalFU.rd] = 2;
                    cpu->zero_flag = FALSE;
                }
                break;
            }
            case OPCODE_XOR:
            {
                cpu->logicalFU.result_buffer
                    = cpu->logicalFU.rs1_value ^ cpu->logicalFU.rs2_value;

                /* Set the zero flag based on the result buffer */
                if (cpu->logicalFU.result_buffer == 0)
                {
                    cpu->zero_flag = TRUE;
                    cpu->ccFlag[cpu->logicalFU.rd] = 1;
                } 
                else 
                {
                    cpu->zero_flag = FALSE;
                    cpu->ccFlag[cpu->logicalFU.rd] = 2;
                }
                break;
            } 
        }

        /* Copy data from execute latch to memory latch*/
        // cpu->writeback = cpu->logicalFU;
        // cpu->logicalFU.has_insn = FALSE;
    if(cpu->logicalFU.allocatedForwardingBus == 1){
        cpu->forwardingBus1.data = cpu->logicalFU.result_buffer;
        cpu->forwardingBus1.tag = cpu->logicalFU.rd;
        cpu->forwardingBus1.applicationStage = cpu->logicalFU;        
        cpu->logicalFU.has_insn = FALSE;
    }
    else if(cpu->logicalFU.allocatedForwardingBus == 2){
        cpu->forwardingBus2.data = cpu->logicalFU.result_buffer;
        cpu->forwardingBus2.tag = cpu->logicalFU.rd;
        cpu->forwardingBus2.applicationStage = cpu->logicalFU;
        cpu->logicalFU.has_insn = FALSE;        
    }

    if(insQueueEntryForwarded != -1){
        if(registerForwarded == 1){
            cpu->insQueue[insQueueEntryForwarded].rs1_value = cpu->logicalFU.result_buffer; 
        } else if(registerForwarded == 2){
            cpu->insQueue[insQueueEntryForwarded].rs2_value = cpu->logicalFU.result_buffer; 
        } else if(registerForwarded == 3){
            cpu->insQueue[insQueueEntryForwarded].rs3_value = cpu->logicalFU.result_buffer; 
        }
    }        
        // if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        // {
        //     print_stage_content("Logical FU", &cpu->logicalFU);
        // }
    }
}


static void APEX_forwarding_Bus1(APEX_CPU *cpu){

    if(cpu->forwardingBus1.tagLock == 1){
        cpu->forwardingBus1.tagLock = 0;
        if(cpu->forwardingBus1.applicationStage.opcode == OPCODE_JUMP){
            printf("Next PC value %d",cpu->pc);
            cpu->fetch.has_insn = 1;
            cpu->fetch_blocked = 0;
            return;
        }
        cpu->writeback = cpu->forwardingBus1.applicationStage;
        if(cpu->forwardingBus1.applicationStage.opcode == OPCODE_LOAD){
            //printf("Did LOAD seriously reach here too???\n");
            //printf("result for LOAD %d\n",cpu->forwardingBus1.applicationStage.result_buffer);
            //printf("PC for LOAD %d\n",cpu->forwardingBus1.applicationStage.pc);
        }
        if(cpu->forwardingBus1.applicationStage.opcode == OPCODE_HALT){
            printf("Halt was also here mann");
        }
        cpu->writeback.has_insn = true;  
        switch (cpu->forwardingBus1.applicationStage.opcode)
        {
            case OPCODE_ADD:      
            case OPCODE_ADDL:           
            case OPCODE_SUB:
            case OPCODE_SUBL:          
            case OPCODE_MUL:        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR: 
            case OPCODE_MOVC:                                               
            case OPCODE_LOAD:
            case OPCODE_LDR:
            {
                cpu->physicalRegisters[cpu->forwardingBus1.applicationStage.rd] = cpu->forwardingBus1.applicationStage.result_buffer;
                // int index=0;
                // for(index=0;index<REG_FILE_SIZE;index++){
                //     //printf("list %d",cpu->renameTable[index]);
                //     if(cpu->renameTable[index] == cpu->writeback.rd){
                //         break;
                //     }
                // }
                //printf("index %d",index);
                //cpu->regs[index] = cpu->writeback.result_buffer;
                break;
            }          
            case OPCODE_STORE:
            {
                //DO nothing in write back stage for store instruction
                break;
            }
            case OPCODE_STR:
            {
                //DO nothing in write back stage for str instruction
                break;
            }            
        }              
    }else if(!isWQEmpty()){
        // if(isWQEmpty()){
        //     return;
        // }
        int busRequest = removeStage(cpu);
        switch (busRequest)
        {
        case 1:
            cpu->forwardingBus1.tag = cpu->intFU.rd;
            cpu->forwardingBus1.data = cpu->intFU.result_buffer;
            cpu->intFU.has_insn = false;
            cpu->forwardingBus1.applicationStage = cpu->intFU;
            if(cpu->forwardingBus1.applicationStage.opcode == OPCODE_JUMP){
                cpu->fetch.has_insn = 1;
                cpu->fetch_blocked = 0;
                return;
            }            
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus1.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus1.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus1.data;
                    break;                    
                }                
            }
            break;
        case 2:
            /* code */
            cpu->forwardingBus1.data = cpu->lsq[lsqHead].applicationStage.result_buffer;
            cpu->forwardingBus1.tag = cpu->lsq[lsqHead].rd;
            cpu->forwardingBus1.applicationStage = cpu->lsq[lsqHead].applicationStage;
            dequeueLSQ(cpu);
            cpu->memory.has_insn = FALSE; 
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus1.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus1.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus1.data;
                    break;                    
                }                
            }                         
            break;
        case 3:
            cpu->forwardingBus1.tag = cpu->mulFU.rd;
            cpu->forwardingBus1.data = cpu->mulFU.result_buffer;
            cpu->mulFU.has_insn = false;
            cpu->forwardingBus1.applicationStage = cpu->mulFU;
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus1.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus1.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus1.data;
                    break;                    
                }                
            }
            break;
        case 4:
            cpu->forwardingBus1.tag = cpu->logicalFU.rd;
            cpu->forwardingBus1.data = cpu->logicalFU.result_buffer;
            cpu->logicalFU.has_insn = false;
            cpu->forwardingBus1.applicationStage = cpu->logicalFU;
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus1.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus1.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus1.data;
                    break;                    
                }                
            }
            break;                                    
        
        default:
            break;
        }
    cpu->writeback = cpu->forwardingBus1.applicationStage;
    cpu->writeback.has_insn = true;  
        switch (cpu->forwardingBus1.applicationStage.opcode)
        {
            case OPCODE_ADD:      
            case OPCODE_ADDL:           
            case OPCODE_SUB:
            case OPCODE_SUBL:          
            case OPCODE_MUL:        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR: 
            case OPCODE_MOVC:                                               
            case OPCODE_LOAD:
            case OPCODE_LDR:
            {
                cpu->physicalRegisters[cpu->forwardingBus1.applicationStage.rd] = cpu->forwardingBus1.applicationStage.result_buffer;
                // int index=0;
                // for(index=0;index<REG_FILE_SIZE;index++){
                //     //printf("list %d",cpu->renameTable[index]);
                //     if(cpu->renameTable[index] == cpu->writeback.rd){
                //         break;
                //     }
                // }
                //printf("index %d",index);
                //cpu->regs[index] = cpu->writeback.result_buffer;
                break;
            }          
            case OPCODE_STORE:
            {
                //DO nothing in write back stage for store instruction
                break;
            }
            case OPCODE_STR:
            {
                //DO nothing in write back stage for str instruction
                break;
            }            
        }          
    }
    //writeback stuff here for now
    //cpu->forwardingBus1.tag = 0;
        
}

static void APEX_forwarding_Bus2(APEX_CPU *cpu){
    if(cpu->forwardingBus2.tagLock == 1){
        cpu->forwardingBus2.tagLock = 0;
        cpu->writeback2 = cpu->forwardingBus2.applicationStage;
        cpu->writeback2.has_insn = true;   

        //write into physical register
        switch (cpu->forwardingBus2.applicationStage.opcode)
        {
            case OPCODE_ADD:      
            case OPCODE_ADDL:           
            case OPCODE_SUB:
            case OPCODE_SUBL:          
            case OPCODE_MUL:        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR: 
            case OPCODE_MOVC:                                               
            case OPCODE_LOAD:
            case OPCODE_LDR:
            {
                cpu->physicalRegisters[cpu->forwardingBus2.applicationStage.rd] = cpu->forwardingBus2.applicationStage.result_buffer;
                // int index=0;
                // for(index=0;index<REG_FILE_SIZE;index++){
                //     //printf("list %d",cpu->renameTable[index]);
                //     if(cpu->renameTable[index] == cpu->writeback.rd){
                //         break;
                //     }
                // }
                //printf("index %d",index);
                //cpu->regs[index] = cpu->writeback.result_buffer;
                break;
            }                      
        }               
    }else if(!isWQEmpty()){
        // if(isWQEmpty()){
        //     return;
        // }
        int busRequest = removeStage(cpu);
        switch (busRequest)
        {
        case 1:
            cpu->forwardingBus2.tag = cpu->intFU.rd;
            cpu->forwardingBus2.data = cpu->intFU.result_buffer;
            cpu->intFU.has_insn = false;
            cpu->forwardingBus2.applicationStage = cpu->intFU;
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus2.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus2.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus2.data;
                    break;                    
                }                
            }
            break;
        case 2:
            cpu->forwardingBus2.data = cpu->lsq[lsqHead].applicationStage.result_buffer;
            cpu->forwardingBus2.tag = cpu->lsq[lsqHead].rd;
            cpu->forwardingBus2.applicationStage = cpu->lsq[lsqHead].applicationStage;
            dequeueLSQ(cpu);
            cpu->memory.has_insn = FALSE;            
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus2.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus2.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus2.data;
                    break;                    
                }                
            }
            break;
        case 3:
            cpu->forwardingBus2.tag = cpu->mulFU.rd;
            cpu->forwardingBus2.data = cpu->mulFU.result_buffer;
            cpu->mulFU.has_insn = false;
            cpu->forwardingBus2.applicationStage = cpu->mulFU;
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus2.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus2.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus2.data;
                    break;                    
                }                
            }
            break;
        case 4:
            cpu->forwardingBus2.tag = cpu->logicalFU.rd;
            cpu->forwardingBus2.data = cpu->logicalFU.result_buffer;
            cpu->logicalFU.has_insn = false;
            cpu->forwardingBus2.applicationStage = cpu->logicalFU;
            //forward to IQ
            for (int i = 0; i < iQCount; i++){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus2.data;
                    break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus2.data;
                    break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus2.data;
                    break;                    
                }                
            }
            break;                                    
        
        default:
            break;
        }        
        cpu->writeback2 = cpu->forwardingBus2.applicationStage;
        cpu->writeback2.has_insn = true; 

        switch (cpu->forwardingBus2.applicationStage.opcode)
        {
            case OPCODE_ADD:      
            case OPCODE_ADDL:           
            case OPCODE_SUB:
            case OPCODE_SUBL:          
            case OPCODE_MUL:        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR: 
            case OPCODE_MOVC:                                               
            case OPCODE_LOAD:
            case OPCODE_LDR:
            {
                cpu->physicalRegisters[cpu->forwardingBus2.applicationStage.rd] = cpu->forwardingBus2.applicationStage.result_buffer;
                // int index=0;
                // for(index=0;index<REG_FILE_SIZE;index++){
                //     //printf("list %d",cpu->renameTable[index]);
                //     if(cpu->renameTable[index] == cpu->writeback.rd){
                //         break;
                //     }
                // }
                //printf("index %d",index);
                //cpu->regs[index] = cpu->writeback.result_buffer;
                break;
            }          
            case OPCODE_STORE:
            {
                //DO nothing in write back stage for store instruction
                break;
            }
            case OPCODE_STR:
            {
                //DO nothing in write back stage for str instruction
                break;
            }            
        }        

    }
    //writeback stuff here for now
    //cpu->forwardingBus1.tag = 0;
}




/*
 * Memory Stage of APEX Pipeline
 *
 * Note: You are free to edit this function according to your implementation
 */
//Mem stage commented for now
static void
APEX_memory(APEX_CPU *cpu)
{    
    if(cpu->forwardingBus1.tag == cpu->lsq[lsqHead].rs1){
        cpu->lsq[lsqHead].rs1ValidBit = 1;
        cpu->lsq[lsqHead].rs1Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[lsqHead].rs1){
        cpu->lsq[lsqHead].rs1ValidBit = 1;
        cpu->lsq[lsqHead].rs1Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[lsqHead].rs2){
        cpu->lsq[lsqHead].rs2ValidBit = 1;
        cpu->lsq[lsqHead].rs2Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[lsqHead].rs2){
        cpu->lsq[lsqHead].rs2ValidBit = 1;
        cpu->lsq[lsqHead].rs2Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[lsqHead].rs3){
        cpu->lsq[lsqHead].rs3ValidBit = 1;
        cpu->lsq[lsqHead].rs3Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[lsqHead].rs3){
        cpu->lsq[lsqHead].rs3ValidBit = 1;
        cpu->lsq[lsqHead].rs3Value = cpu->forwardingBus2.data;
    }

    if(lsqTail >= lsqHead)
    {
        for(int i = lsqHead; i <= lsqTail; i++)
        {
    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs1){
        cpu->lsq[i].rs1ValidBit = 1;
        cpu->lsq[i].rs1Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs1){
        cpu->lsq[i].rs1ValidBit = 1;
        cpu->lsq[i].rs1Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs2){
        cpu->lsq[i].rs2ValidBit = 1;
        cpu->lsq[i].rs2Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs2){
        cpu->lsq[i].rs2ValidBit = 1;
        cpu->lsq[i].rs2Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs3){
        cpu->lsq[i].rs3ValidBit = 1;
        cpu->lsq[i].rs3Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs3){
        cpu->lsq[i].rs3ValidBit = 1;
        cpu->lsq[i].rs3Value = cpu->forwardingBus2.data;
    }
        }
    }
    else
    {    
        for(int i = lsqHead; i < LSQ_SIZE; i++)
        {  
    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs1){
        cpu->lsq[i].rs1ValidBit = 1;
        cpu->lsq[i].rs1Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs1){
        cpu->lsq[i].rs1ValidBit = 1;
        cpu->lsq[i].rs1Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs2){
        cpu->lsq[i].rs2ValidBit = 1;
        cpu->lsq[i].rs2Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs2){
        cpu->lsq[i].rs2ValidBit = 1;
        cpu->lsq[i].rs2Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs3){
        cpu->lsq[i].rs3ValidBit = 1;
        cpu->lsq[i].rs3Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs3){
        cpu->lsq[i].rs3ValidBit = 1;
        cpu->lsq[i].rs3Value = cpu->forwardingBus2.data;
    }
        }

        for(int i = 0; i <= lsqTail; i++)
        {
    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs1){
        cpu->lsq[i].rs1ValidBit = 1;
        cpu->lsq[i].rs1Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs1){
        cpu->lsq[i].rs1ValidBit = 1;
        cpu->lsq[i].rs1Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs2){
        cpu->lsq[i].rs2ValidBit = 1;
        cpu->lsq[i].rs2Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs2){
        cpu->lsq[i].rs2ValidBit = 1;
        cpu->lsq[i].rs2Value = cpu->forwardingBus2.data;
    }

    if(cpu->forwardingBus1.tag == cpu->lsq[i].rs3){
        cpu->lsq[i].rs3ValidBit = 1;
        cpu->lsq[i].rs3Value = cpu->forwardingBus1.data;
    }else if(cpu->forwardingBus2.tag == cpu->lsq[i].rs3){
        cpu->lsq[i].rs3ValidBit = 1;
        cpu->lsq[i].rs3Value = cpu->forwardingBus2.data;
    }
        }
    }


    //cpu->rob[robHead].pc == cpu->lsq[lsqHead].pc &&     
    if(!cpu->memory.has_insn && robElementCount>0 && lsqElementCount>0 && cpu->rob[robHead].pc == cpu->lsq[lsqHead].pc && (cpu->lsq[lsqHead].rs1ValidBit == 1 && cpu->lsq[lsqHead].rs2ValidBit == 1 
                                                        && cpu->lsq[lsqHead].rs3ValidBit == 1)){
            cpu->memory.has_insn = true;
            // if(cpu->lsq[lsqHead].instructionOpcode == OPCODE_LOAD){
            //     printf("do we reach here in case of LOAD");
            // }            
            //printf("do we reach here in case of LOAD");
            //dequeueLSQ(cpu);
    }
    if (cpu->memory.has_insn && (ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP)){
        print_stage_content("D Cache FU", &cpu->memory);
    }else{
        if(ENABLE_DEBUG_MESSAGES || cpu->argument1 == SINGLE_STEP){
            char name[10] = "D Cache FU";
            printf("%-15s:", name);
            printf("Empty");
            printf("\n");  
        }
    }
    if (cpu->memory.has_insn){
       switch (cpu->lsq[lsqHead].instructionOpcode){
            case OPCODE_LOAD:
            {
                cpu->memory.allocatedForwardingBus = 0;
                cpu->lsq[lsqHead].applicationStage.opcode = OPCODE_LOAD;
                cpu->lsq[lsqHead].applicationStage.pc = cpu->lsq[lsqHead].pc;
                cpu->lsq[lsqHead].applicationStage.rs1 = cpu->lsq[lsqHead].rs1;
                cpu->lsq[lsqHead].applicationStage.rd = cpu->lsq[lsqHead].rd;
                cpu->lsq[lsqHead].applicationStage.imm = cpu->lsq[lsqHead].imm;
                cpu->lsq[lsqHead].memory_address = cpu->lsq[lsqHead].rs1Value + cpu->lsq[lsqHead].imm;
                cpu->lsq[lsqHead].applicationStage.result_buffer = cpu->data_memory[cpu->lsq[lsqHead].memory_address];  
                if(isWQEmpty() && cpu->forwardingBus1.tagLock!=1){
                    cpu->forwardingBus1.tagLock=1;
                    cpu->forwardingBus1.data = cpu->lsq[lsqHead].applicationStage.result_buffer;
                    cpu->forwardingBus1.tag = cpu->lsq[lsqHead].rd;
                    cpu->forwardingBus1.applicationStage = cpu->lsq[lsqHead].applicationStage;
                    dequeueLSQ(cpu);
                    cpu->memory.has_insn = FALSE;    
                    cpu->memory.allocatedForwardingBus = 1;  
                    //printf("we here for load?");                
                }
                else if(isWQEmpty() && cpu->forwardingBus2.tagLock!=1){
                    cpu->forwardingBus2.tagLock=1;
                    cpu->forwardingBus2.data = cpu->lsq[lsqHead].applicationStage.result_buffer;
                    cpu->forwardingBus2.tag = cpu->lsq[lsqHead].rd;
                    cpu->forwardingBus2.applicationStage = cpu->lsq[lsqHead].applicationStage;
                    dequeueLSQ(cpu);
                    cpu->memory.has_insn = FALSE;  
                    cpu->memory.allocatedForwardingBus = 2;   
                }else{
                    insertWQ(cpu,2);
                }                              

        for (int i = 0; i < iQCount; i++)
        {
            if(cpu->memory.allocatedForwardingBus == 1){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus1.data;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus1.data;
                    //break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus1.data;
                    //break;                    
                }                                
            }else if(cpu->memory.allocatedForwardingBus == 2){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus2.data;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus2.data;
                    //break;                    
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus2.data;
                    //break;                    
                }                                
            }
        }
                //int result_buffer = cpu->data_memory[cpu->lsq[lsqHead].memory_address];             
                break;
            }
            case OPCODE_LDR:
            {
                cpu->memory.allocatedForwardingBus = 0;
                cpu->lsq[lsqHead].applicationStage.opcode = OPCODE_LDR;
                cpu->lsq[lsqHead].applicationStage.pc = cpu->lsq[lsqHead].pc;
                cpu->lsq[lsqHead].memory_address = cpu->lsq[lsqHead].rs1Value + cpu->lsq[lsqHead].rs2Value;
                cpu->lsq[lsqHead].applicationStage.rs1 = cpu->lsq[lsqHead].rs1;
                cpu->lsq[lsqHead].applicationStage.rd = cpu->lsq[lsqHead].rd;
                cpu->lsq[lsqHead].applicationStage.rs2 = cpu->lsq[lsqHead].rs2;                
                cpu->lsq[lsqHead].applicationStage.result_buffer = cpu->data_memory[cpu->lsq[lsqHead].memory_address]; 
                if(isWQEmpty() && cpu->forwardingBus1.tagLock!=1){
                    cpu->forwardingBus1.tagLock=1;
                    cpu->forwardingBus1.data = cpu->lsq[lsqHead].applicationStage.result_buffer;
                    cpu->forwardingBus1.tag = cpu->lsq[lsqHead].rd;
                    cpu->forwardingBus1.applicationStage = cpu->lsq[lsqHead].applicationStage;
                    dequeueLSQ(cpu);
                    cpu->memory.has_insn = FALSE;    
                    cpu->memory.allocatedForwardingBus = 1;                  
                }
                else if(isWQEmpty() && cpu->forwardingBus2.tagLock!=1){
                    cpu->forwardingBus2.tagLock=1;
                    cpu->forwardingBus2.data = cpu->lsq[lsqHead].applicationStage.result_buffer;
                    cpu->forwardingBus2.tag = cpu->lsq[lsqHead].rd;
                    cpu->forwardingBus2.applicationStage = cpu->lsq[lsqHead].applicationStage;
                    dequeueLSQ(cpu);
                    cpu->memory.has_insn = FALSE;  
                    cpu->memory.allocatedForwardingBus = 2;   
                }else{
                    insertWQ(cpu,2);
                }                              

        for (int i = 0; i < iQCount; i++)
        {
            if(cpu->memory.allocatedForwardingBus == 1){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus1.data;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus1.data;
                    //break;                 
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus1.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus1.data;
                    //break;                    
                }                                
            }else if(cpu->memory.allocatedForwardingBus == 2){
                if(cpu->insQueue[i].rs1_valid == 0 && cpu->insQueue[i].rs1_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs1_valid = 1;
                    cpu->insQueue[i].rs1_value = cpu->forwardingBus2.data;
                    //break;                    
                }
                if(cpu->insQueue[i].rs2_valid == 0 && cpu->insQueue[i].rs2_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs2_valid = 1;
                    cpu->insQueue[i].rs2_value = cpu->forwardingBus2.data;
                    //break;                    
                }                
                if(cpu->insQueue[i].rs3_valid == 0 && cpu->insQueue[i].rs3_tag == cpu->forwardingBus2.tag){
                    cpu->insQueue[i].rs3_valid = 1;
                    cpu->insQueue[i].rs3_value = cpu->forwardingBus2.data;
                    //break;                    
                }                                
            }
        }                               
                //int result_buffer = cpu->data_memory[cpu->lsq[lsqHead].memory_address];                  
                break;
            }            
            case OPCODE_STORE:
            {
                cpu->lsq[lsqHead].memory_address = cpu->lsq[lsqHead].rs2Value + cpu->lsq[lsqHead].imm;
                cpu->data_memory[cpu->lsq[lsqHead].memory_address] =  cpu->lsq[lsqHead].rs1Value;
                dequeueROB(cpu);
                dequeueLSQ(cpu);
                cpu->memory.has_insn = false;                    
                break;
            }
            case OPCODE_STR:
            {
                cpu->lsq[lsqHead].memory_address = cpu->lsq[lsqHead].rs2Value + cpu->lsq[lsqHead].rs1Value;
                cpu->data_memory[cpu->lsq[lsqHead].memory_address] =  cpu->lsq[lsqHead].rs3Value;
                dequeueROB(cpu);
                dequeueLSQ(cpu);
                cpu->memory.has_insn = false;                    
                break;
            }                
       } 
    }     

    // if (cpu->memory.has_insn)
    // {           
    //     switch (cpu->memory.opcode)
    //     {                                               
    //         case OPCODE_LOAD:
    //         {
    //             /* Read from data memory */
    //             cpu->memory.result_buffer
    //                 = cpu->data_memory[cpu->memory.memory_address];
    //             //printf("*********MY text MEMORY%d\n\n",cpu->memory.memory_address);                    
    //             break;
    //         }
    //         case OPCODE_LDR:
    //         {
    //             /* Read from data memory */
    //             cpu->memory.result_buffer
    //                 = cpu->data_memory[cpu->memory.memory_address];
    //             //printf("*********MY text MEMORY%d\n\n",cpu->memory.memory_address);                    
    //             break;
    //         }            
    //         case OPCODE_STORE:
    //         {
    //             /* Read from data memory */
    //             cpu->data_memory[cpu->memory.memory_address] = 
    //             cpu->memory.rs1_value;
    //             //printf("*****Result for STORE instruction******* %d   %d",cpu->data_memory[cpu->memory.memory_address],cpu->memory.memory_address);
    //             break;
    //         }
    //         case OPCODE_STR:
    //         {
    //             /* Read from data memory */
    //             cpu->data_memory[cpu->memory.memory_address] = 
    //             cpu->memory.rs3_value;
    //             //printf("*****Result for STR instruction******* %d   %d",cpu->data_memory[cpu->memory.memory_address],cpu->memory.memory_address);
    //             break;
    //         }                        
    //     }

    //     /* Copy data from memory latch to writeback latch*/
    //     cpu->writeback = cpu->memory;
    //     cpu->memory.has_insn = FALSE;

    //     if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
    //     {
    //         print_stage_content("Memory", &cpu->memory);
    //     }
    // }
}

/*
 * Writeback Stage of APEX Pipeline
 *
 * Note: You are free to edit this function according to your implementation
 */
static int
APEX_writeback(APEX_CPU *cpu)
{
    if(robElementCount>0 && cpu->rob[robHead].instructionOpcode == OPCODE_HALT){
        cpu->writeback.opcode = OPCODE_HALT;
        return TRUE;
    }    
    if (cpu->writeback.has_insn)
    {
        //printf("ROB head tail and element coount %d %d %d ",robHead,robTail,robElementCount);
    if(robTail >= robHead)
    {
     
        // Loop to print elements from
        // front to rear.
        for(int i = robHead; i <= robTail; i++)
        {
            //printf("ROB ENTRY pc at the head pc %d\n",cpu->rob[i].pc);
            if(cpu->rob[i].pc == cpu->writeback.pc){
                cpu->rob[i].pdValidBit = 1;
            }
        }
    }
 
    // If rear crossed the max index and
    // indexing has started in loop
    else
    {      
        // Loop for printing elements from
        // front to max size or last index
        for(int i = robHead; i < ROB_SIZE; i++)
        {
            //printf("ROB ENTRY pc at the head pc %d\n",cpu->rob[i].pc);
            if(cpu->rob[i].pc == cpu->writeback.pc){
                cpu->rob[i].pdValidBit = 1;
            }            
        }
 
        // Loop for printing elements from
        // 0th index till rear position
        for(int i = 0; i <= robTail; i++)
        {
            //printf("ROB ENTRY pc at the head pc %d\n",cpu->rob[i].pc);
            if(cpu->rob[i].pc == cpu->writeback.pc){
                cpu->rob[i].pdValidBit = 1;
            }            
        }
    }
    // printf("rob robhead pd valid bit %d\n",cpu->rob[robHead].pdValidBit);
    // printf("rob robhead pd valid bit %d\n",cpu->writeback.pc);
    // if(cpu->rob[robHead].pc == 4040){
    //    printf("4040 instruction debug in writeback %d\n",cpu->rob[robHead].pdValidBit); 
    // }    
    if(cpu->rob[robHead].pdValidBit == 1){
    // if(cpu->rob[robHead].pc == 4040){
    //    printf("4040 instruction debug in writeback %d\n",cpu->rob[robHead].pdValidBit); 
    // }        
        if(robElementCount>0 && cpu->rob[robHead].instructionOpcode == OPCODE_HALT){
            return TRUE;
        }
        // cpu->rob[robHead]
        /* Write result to register file based on instruction type */
        switch (cpu->rob[robHead].instructionOpcode)
        {
            case OPCODE_ADD:      
            case OPCODE_ADDL:           
            case OPCODE_SUB:
            case OPCODE_SUBL:          
            case OPCODE_MUL:        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR: 
            case OPCODE_MOVC:                                               
            case OPCODE_CMP: 
            case OPCODE_LOAD:
            case OPCODE_LDR:
            {
                //cpu->physicalRegisters[cpu->rob[robHead].pd] = cpu->writeback.result_buffer;
                // int index=0;
                // for(index=0;index<REG_FILE_SIZE;index++){
                //     //printf("list %d",cpu->renameTable[index]);
                //     if(cpu->renameTable[index] == cpu->writeback.rd){
                //         break;
                //     }
                // }
                //printf("index %d",index);
                //cpu->regs[index] = cpu->writeback.result_buffer;
                cpu->regs[cpu->rob[robHead].rd] = cpu->writeback.result_buffer;
                insert(cpu,cpu->rob[robHead].overwrittenRenameTableEntry);
                break;
            }          
            case OPCODE_STORE:
            {
                //DO nothing in write back stage for store instruction
                break;
            }
            case OPCODE_STR:
            {
                //DO nothing in write back stage for str instruction
                break;
            }            
        }
        //remove head from the ROB
        //printf("removing the head\n");
        dequeueROB(cpu);
    }        

        cpu->insn_completed++;
        cpu->writeback.has_insn = FALSE;

        if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        {
            print_stage_content("ROB stage", &cpu->writeback);
        }

        if (robElementCount>0 && cpu->rob[robHead].instructionOpcode == OPCODE_HALT)
        {
            /* Stop the APEX simulator */
            return TRUE;
        }
        /*making the destination registers available for the instructions waiting for accessing these registers*/
        //cpu->state_regs[cpu->writeback.rd] = 0;
    }

    /* Default */
    return 0;
}



static int
APEX_writeback2(APEX_CPU *cpu)
{
    if(robElementCount > 0 && cpu->rob[robHead].pdValidBit == 1){
    // if(cpu->rob[robHead].pc == 4040){
    //    printf("4040 instruction debug in writeback inside if %d\n",cpu->rob[robHead].pdValidBit); 
    // }
        if(robElementCount>0 && cpu->rob[robHead].instructionOpcode == OPCODE_HALT){
            return TRUE;
        }
        // cpu->rob[robHead]
        /* Write result to register file based on instruction type */
        switch (cpu->rob[robHead].instructionOpcode)
        {
            case OPCODE_ADD:      
            case OPCODE_ADDL:           
            case OPCODE_SUB:
            case OPCODE_SUBL:          
            case OPCODE_MUL:        
            case OPCODE_AND:
            case OPCODE_OR:
            case OPCODE_XOR: 
            case OPCODE_CMP:
            case OPCODE_MOVC:                                               
            case OPCODE_LOAD:
            case OPCODE_LDR:
            {
                //cpu->physicalRegisters[cpu->rob[robHead].pd] = cpu->writeback.result_buffer;
                // int index=0;
                // for(index=0;index<REG_FILE_SIZE;index++){
                //     //printf("list %d",cpu->renameTable[index]);
                //     if(cpu->renameTable[index] == cpu->writeback.rd){
                //         break;
                //     }
                // }
                //printf("index %d",index);
                //cpu->regs[index] = cpu->writeback.result_buffer;
                cpu->regs[cpu->rob[robHead].rd] = cpu->writeback2.result_buffer;
                insert(cpu,cpu->rob[robHead].overwrittenRenameTableEntry);
                break;
            }          
            case OPCODE_STORE:
            {
                //DO nothing in write back stage for store instruction
                break;
            }
            case OPCODE_STR:
            {
                //DO nothing in write back stage for str instruction
                break;
            }            
        }
        //remove head from the ROB
        printf("removing the head\n");
        dequeueROB(cpu);
    }    

    if (cpu->writeback2.has_insn)
    {
    if(robTail >= robHead)
    {
     
        // Loop to print elements from
        // front to rear.
        for(int i = robHead; i <= robTail; i++)
        {
            //printf("ROB ENTRY pc at the head pc %d\n",cpu->rob[i].pc);
            if(cpu->rob[i].pc == cpu->writeback2.pc){
                cpu->rob[i].pdValidBit = 1;
            }
        }
    }
 
    // If rear crossed the max index and
    // indexing has started in loop
    else
    {      
        // Loop for printing elements from
        // front to max size or last index
        for(int i = robHead; i < ROB_SIZE; i++)
        {
            //printf("ROB ENTRY pc at the head pc %d\n",cpu->rob[i].pc);
            if(cpu->rob[i].pc == cpu->writeback2.pc){
                cpu->rob[i].pdValidBit = 1;
            }            
        }
 
        // Loop for printing elements from
        // 0th index till rear position
        for(int i = 0; i <= robTail; i++)
        {
            //printf("ROB ENTRY pc at the head pc %d\n",cpu->rob[i].pc);
            if(cpu->rob[i].pc == cpu->writeback2.pc){
                cpu->rob[i].pdValidBit = 1;
            }            
        }
    }
    // printf("rob robhead pd valid bit %d\n",robTail);
    // printf("rob robhead pd valid bit %d\n",cpu->writeback2.pc);
    // if(cpu->rob[robHead].pc == 4040){
    //    printf("4040 instruction debug in writeback %d\n",cpu->rob[robHead].pdValidBit); 
    // }
        

        cpu->insn_completed++;
        cpu->writeback2.has_insn = FALSE;

        if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        {
            print_stage_content("ROB stage", &cpu->writeback2);
        }

        if (robElementCount>0 && cpu->rob[robHead].instructionOpcode == OPCODE_HALT)
        {
            /* Stop the APEX simulator */
            return TRUE;
        }
        /*making the destination registers available for the instructions waiting for accessing these registers*/
        //cpu->state_regs[cpu->writeback.rd] = 0;
    }

    /* Default */
    return 0;
}


/*
 * This function creates and initializes APEX cpu.
 *
 * Note: You are free to edit this function according to your implementation
 */
APEX_CPU *
APEX_cpu_init(const char *filename)
{
    int i;
    APEX_CPU *cpu;
    //Instruction_Queue *insQueue;

    if (!filename)
    {
        return NULL;
    }

    cpu = calloc(1, sizeof(APEX_CPU));
    //insQueue = calloc(Instruction_QUEUE_SIZE, sizeof(APEX_CPU));
    if (!cpu)
    {
        return NULL;
    }

    /* Initialize PC, Registers and all pipeline stages */
    cpu->pc = 4000;
    //newly added line for testing
    memset(cpu->regs, 0, sizeof(int) * REG_FILE_SIZE);
    memset(cpu->renameTable, 0, sizeof(int) * (REG_FILE_SIZE+1));
    memset(cpu->physicalRegisters, 0, sizeof(int) * (PHY_REG_FILE_SIZE));
    memset(cpu->freeListQueue, 0, sizeof(int) * (FREELIST_QUEUE_SIZE));
    //memset(cpu->state_regs, 0, sizeof(int) * REG_FILE_SIZE); //newly added line
    memset(cpu->data_memory, 0, sizeof(int) * DATA_MEMORY_SIZE);
    //cpu->data_memory[10] = 10;    
    cpu->single_step = ENABLE_SINGLE_STEP;

    /* Parse input file and create code memory */
    cpu->code_memory = create_code_memory(filename, &cpu->code_memory_size);
    for (int index = 0; index < (REG_FILE_SIZE + 1); index++)
    {
        cpu->renameTable[index] = index+1;
    }
    
    if (!cpu->code_memory)
    {
        free(cpu);
        return NULL;
    }

    if (ENABLE_DEBUG_MESSAGES)
    {
        fprintf(stderr,
                "APEX_CPU: Initialized APEX CPU, loaded %d instructions\n",
                cpu->code_memory_size);
        fprintf(stderr, "APEX_CPU: PC initialized to %d\n", cpu->pc);
        fprintf(stderr, "APEX_CPU: Printing Code Memory\n");
        printf("%-9s %-9s %-9s %-9s %-9s\n", "opcode_str", "rd", "rs1", "rs2",
               "imm");

        for (i = 0; i < cpu->code_memory_size; ++i)
        {
            printf("%-9s %-9d %-9d %-9d %-9d\n", cpu->code_memory[i].opcode_str,
                   cpu->code_memory[i].rd, cpu->code_memory[i].rs1,
                   cpu->code_memory[i].rs2, cpu->code_memory[i].imm);
        }
    }

    /* To start fetch stage */
    cpu->fetch.has_insn = TRUE;
    return cpu;
}

/*
 * APEX CPU simulation loop
 *
 * Note: You are free to edit this function according to your implementation
 */
void
APEX_cpu_run(APEX_CPU *cpu)
{
    char user_prompt_val;
    int numberOfCycle;
    insert(cpu,9);
    insert(cpu,10);
    insert(cpu,11);
    insert(cpu,12);
    insert(cpu,13);
    insert(cpu,14);
    while (TRUE)
    {
        if (ENABLE_DEBUG_MESSAGES || cpu->argument1 == DISPLAY)
        {
            printf("--------------------------------------------\n");
            printf("Clock Cycle #: %d\n", cpu->clock);
            printf("--------------------------------------------\n");
        }

        if (APEX_writeback(cpu) || APEX_writeback2(cpu))
        {
            // printf("ROB head tail and element coount in the end %d %d %d ",robHead,robTail,robElementCount);
            // printf("ROB head %d",cpu->rob[robHead].pc);
            /* Halt in writeback stage */
                 print_reg_file(cpu);
                 print_mem_file(cpu);            
            if(cpu->argument1 == DISPLAY){
            }
            if(cpu->argument1 == SHOWMEM ){
                if(cpu->argument2>=DATA_MEMORY_SIZE){
                    printf("Please enter valid memory address");
                    print_reg_file(cpu);
                }else{
                    print_reg_file(cpu);
                    print_mem_file(cpu);
                    printf("MEM%-3d[%-3d] ", cpu->argument2, cpu->data_memory[cpu->argument2]);
                }
            }
            printf("APEX_CPU: Simulation Complete, cycles = %d instructions = %d\n", cpu->clock, cpu->insn_completed);
            break;
        }

        //APEX_memory(cpu);
        APEX_forwarding_Bus1(cpu);
        APEX_forwarding_Bus2(cpu);
        APEX_logicalFU(cpu);
        APEX_mulFU(cpu);
        APEX_mulFU3(cpu);
        APEX_mulFU2(cpu);
        APEX_mulFU1(cpu);
        APEX_memory(cpu);
        APEX_intFU(cpu);
        print_ins_queue(cpu);
        APEX_renameDispatch(cpu);
        APEX_decode(cpu);
        APEX_fetch(cpu);

        //print_reg_file(cpu);

        if (cpu->single_step)
        {
            printROB(cpu);
            printLSQ(cpu);
            printINSQ(cpu);
            print_phy_reg_file(cpu);
            printf("Press any key to advance CPU Clock or <q> to quit:\n");
            scanf("%c", &user_prompt_val);

            if ((user_prompt_val == 'Q') || (user_prompt_val == 'q'))
            {
                printf("APEX_CPU: Simulation Stopped, cycles = %d instructions = %d\n", cpu->clock, cpu->insn_completed);
                break;
            }
            print_reg_file(cpu);
            print_mem_file(cpu);             
            //print_phy_reg_file(cpu);
        }

        if(cpu->argument2!=0 && cpu->argument1==SIMULATE && cpu->argument2 == cpu->clock){
            print_reg_file(cpu);
            print_mem_file(cpu);
            printf("Press enter next number of cycles to run or enter 0 to quit\n");
            scanf("%d", &numberOfCycle);
            if(numberOfCycle ==0){
                printf("APEX_CPU: Simulation Stopped, cycles = %d instructions = %d\n", cpu->clock, cpu->insn_completed);
                break;                
            }else{
                cpu->argument2=cpu->argument2+numberOfCycle;
            }
            
        }
        //For Display
        if(cpu->argument2!=0 && cpu->argument1==DISPLAY && cpu->argument2 == cpu->clock){
            print_reg_file(cpu);
            print_mem_file(cpu);
            printROB(cpu);
            printLSQ(cpu);
            printINSQ(cpu);
            print_phy_reg_file(cpu);
            printf("Press enter next number of cycles to run or enter 0 to quit\n");
            scanf("%d", &numberOfCycle);
            if(numberOfCycle ==0){
                printf("APEX_CPU: Simulation Stopped, cycles = %d instructions = %d\n", cpu->clock, cpu->insn_completed);
                break;                
            }else{
                cpu->argument2=cpu->argument2+numberOfCycle;
            }
            
        }        
        cpu->clock++;
    }
}

/*
 * This function deallocates APEX CPU.
 *
 * Note: You are free to edit this function according to your implementation
 */
void
APEX_cpu_stop(APEX_CPU *cpu)
{
    free(cpu->code_memory);
    free(cpu);
}

int APEX_cpu_simulate(char const *command){
    if (strcmp(command, "initialize") == 0){
        return INITIALIZE;
    }
    if (strcmp(command, "simulate") == 0){
        return SIMULATE;
    }
    if (strcmp(command, "single_step") == 0){
        return SINGLE_STEP;
    }
    if (strcmp(command, "display") == 0){
        return DISPLAY;
    }
    if (strcmp(command, "showmem") == 0){
        return SHOWMEM;
    }    
    else return 0;            
}