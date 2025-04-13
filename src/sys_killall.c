/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */
//Add
 #include "string.h"
#include "queue.h"
//End-dd
#include "common.h"
#include "syscall.h"
#include "stdio.h"
#include "libmem.h"

int __sys_killall(struct pcb_t *caller, struct sc_regs* regs)
{
    char proc_name[100];
    uint32_t data;

    //hardcode for demo only
    uint32_t memrg = regs->a1;
    
    /* TODO: Get name of the target proc */
    //proc_name = libread..
    int i = 0;
    data = 0;
    while(data != -1){
        libread(caller, memrg, i, &data);
        proc_name[i]= data;
        if(data == -1) proc_name[i]='\0';
        i++;
    }
    printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);

    /* TODO: Traverse proclist to terminate the proc
     *       stcmp to check the process match proc_name
     */
    //free proc trong runnoing list
    struct queue_t *runninglist = caller->running_list;
    int take_idx = -1;
    for (int i = 0; i < runninglist->size; i++){
        struct pcb_t *take_proc = runninglist->proc[i];
        if (strstr(take_proc->path, proc_name)){
            take_idx = i;
            break;
        }
    }
    if (take_idx != -1){
        struct pcb_t *take_proc = runninglist->proc[take_idx];
        for(int i = take_idx + 1; i < runninglist->size; i++){
            runninglist->proc[i-1] = runninglist->proc[i];
        }
        free(take_proc);
        runninglist->size--;
    } 
    //free proc trong mlq_ready_list
    struct queue_t *mlq_ready_q = caller->mlq_ready_queue;
    int take_idx2 = -1;
    for (int i = 0; i < mlq_ready_q->size; i++){
        struct pcb_t *take_proc = mlq_ready_q->proc[i];
        if (strstr(take_proc->path, proc_name)){
            take_idx2 = i;
            break;
        }
    }
    if (take_idx2 != -1){
        struct pcb_t *take_proc = mlq_ready_q->proc[take_idx2];
        for(int i = take_idx2 + 1; i < mlq_ready_q->size; i++){
            mlq_ready_q->proc[i-1] = mlq_ready_q->proc[i];
        }
        free(take_proc);
        mlq_ready_q->size--;
    }
    /* TODO Maching and terminating 
     *       all processes with given
     *        name in var proc_name
     */



    return 0; 
}
