/*
 * apex_cpu.h
 * Contains APEX cpu pipeline declarations
 *
 * Author:
 * Copyright (c) 2020, Gaurav Kothari (gkothar1@binghamton.edu)
 * State University of New York at Binghamton
 */
#ifndef _APEX_CPU_H_
#define _APEX_CPU_H_

#include "apex_macros.h"

/* Format of an APEX instruction  */
typedef struct APEX_Instruction
{
    char opcode_str[128];
    int opcode;
    int rd;
    int rs1;
    int rs2;
    int rs3;
    int imm;
} APEX_Instruction;


/* Format of an APEX instruction  */
typedef struct Instruction_Queue
{
    int pc;
    int validBit;
    char opcode_str[128];
    int opcode;
    int imm;
    int rs1_valid;
    int rs1_tag;
    int rs1_value;
    int rs2_valid;
    int rs2_tag;
    int rs2_value;  
    int rs3_valid;
    int rs3_tag;
    int rs3_value;        
    int rd;  
    int registerRenamed;
    int arcDest;  
    int branchLinkAddr;
} Instruction_Queue;

/* Model of CPU stage latch */
typedef struct CPU_Stage
{
    int pc;
    char opcode_str[128];
    int opcode;
    int rs1;
    int rs2;
    int rs3;
    int rd;
    int imm;
    int rs1_value;
    int rs2_value;
    int rs3_value;
    int result_buffer;
    int memory_address;
    int has_insn;
    int registerRenamed;
    int arcDest;
    int noBusAllocated;
    int allocatedForwardingBus;
    int branchLinkAddr;
} CPU_Stage;

/* Format of an Forwarding bus instruction  */
typedef struct APEX_Forwarding_Bus
{
    int tag;
    int tagLock;
    int data;
    int dataLock;
    CPU_Stage applicationStage;
} APEX_Forwarding_Bus;

/* Format of an ROB  */
typedef struct ROB
{
    int robEntryEstablishedBit;
    int instructionOpcode;
    int pc;
    int pdValidBit;
    int pd;
    int overwrittenRenameTableEntry;
    int rd;
    int lsqIndex;
    int memoryErrorCodes;
} ROB;

/* Format of an LSQ  */
typedef struct LSQ
{
    int lsqEntryEstablishedBit;
    int instructionOpcode;
    int pc;
    int rd;
    int rs1ValidBit;
    int rs1;
    int rs1Value;
    int rs2ValidBit;
    int rs2;
    int rs2Value;
    int rs3ValidBit;
    int rs3;
    int rs3Value;
    int imm;  
    int memory_address;
    int registerRenamed;
    int arcDest;

    CPU_Stage applicationStage;          
} LSQ;

/* Model of APEX CPU */
typedef struct APEX_CPU
{
    int pc;                        /* Current program counter */
    int clock;                     /* Clock cycles elapsed */
    int insn_completed;            /* Instructions retired */
    int regs[REG_FILE_SIZE];       /* Integer register file */
//    int state_regs[REG_FILE_SIZE];       /* State register file */
    int code_memory_size;          /* Number of instruction in the input file */
    APEX_Instruction *code_memory; /* Code Memory */
    int data_memory[DATA_MEMORY_SIZE]; /* Data Memory */
    int single_step;               /* Wait for user input after every cycle */
    int zero_flag;                 /* {TRUE, FALSE} Used by BZ and BNZ to branch */
    int fetch_from_next_cycle;
    int fetch_decode_from_next_cycle;
    int fetch_blocked;
    int argument1;                  /* Used to capture argument 1 from user */
    int argument2;                  /* Used to capture argument 2 from user */
    int renameTable[REG_FILE_SIZE+1];                /* Rename Table added */
    int physicalRegisters[PHY_REG_FILE_SIZE];       /*  Physical Registers for Renaming     */
    int freeListQueue[FREELIST_QUEUE_SIZE]; 
    int ccFlag[PHY_REG_FILE_SIZE];
    //Instruction Queue
    Instruction_Queue newInsQueueEntry;
    Instruction_Queue insQueue[Instruction_QUEUE_SIZE];
    int waitingQueue[Waiting_QUEUE_SIZE];
    APEX_Forwarding_Bus forwardingBus1;
    APEX_Forwarding_Bus forwardingBus2;
    ROB rob[ROB_SIZE];
    ROB newROBEntry;
    LSQ lsq[LSQ_SIZE];
    LSQ newLSQEntry;

    /* Pipeline stages */
    CPU_Stage fetch;
    CPU_Stage decode;
    CPU_Stage renameDispatch;          /*   Rename Dispatch stage added     */
    CPU_Stage intFU;
    CPU_Stage logicalFU;
    CPU_Stage mulFU;
    CPU_Stage mul1FU;
    CPU_Stage mul2FU;
    CPU_Stage mul3FU;
    CPU_Stage memory;
    CPU_Stage writeback;
    CPU_Stage writeback2;
} APEX_CPU;

APEX_Instruction *create_code_memory(const char *filename, int *size);
APEX_CPU *APEX_cpu_init(const char *filename);
void APEX_cpu_run(APEX_CPU *cpu);
void APEX_cpu_stop(APEX_CPU *cpu);
int APEX_cpu_simulate(char const *command);
#endif
