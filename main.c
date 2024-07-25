/*
 * main.c
 *
 * Author:
 * Copyright (c) 2020, Gaurav Kothari (gkothar1@binghamton.edu)
 * State University of New York at Binghamton
 */
#include <stdio.h>
#include <stdlib.h>

#include "apex_cpu.h"

int
main(int argc, char const *argv[])
{
    APEX_CPU *cpu;

    fprintf(stderr, "APEX CPU Pipeline Simulator v%0.1lf\n", VERSION);

    // if (argc != 2)
    // {
    //     fprintf(stderr, "APEX_Help: Usage %s <input_file>\n", argv[0]);
    //     exit(1);
    // }
    int command=0,command2=0;
    if(argc == 3 || argc == 4){
        command = APEX_cpu_simulate(argv[2]);
        if(argc == 4){
            command2=atoi(argv[3]);
        }
    }
    cpu = APEX_cpu_init(argv[1]);
    if (!cpu)
    {
        fprintf(stderr, "APEX_Error: Unable to initialize CPU\n");
        exit(1);
    }
    cpu->argument1 = command;
    cpu->argument2 = command2;
    if(command == SINGLE_STEP || argc == 2){
        cpu->single_step = 1;
    }
    if(command != INITIALIZE){
        APEX_cpu_run(cpu);
        APEX_cpu_stop(cpu);        
    }
    return 0;
}