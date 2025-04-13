#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */
        if(proc != NULL && q != NULL && q->size < MAX_QUEUE_SIZE){
                q->proc[q->size] = proc;
                q->proc[q->size]->priority = proc->prio;
                q->size++;
        }
}

struct pcb_t * dequeue(struct queue_t * q) {
        /* TODO: return a pcb whose prioprity is the highest
         * in the queue [q] and remember to remove it from q
         * */
        if (q == NULL || q->size == 0) {
        return NULL;  
        }
        
        //xac dinh proc co highestPrio
        int highestPrio = q->proc[0]->priority;
        // printf("%d\n",highestPrio);
        int take_idx = 0;
        for (int i = 1; i < q->size; i++){
                if (q->proc[i]->priority < highestPrio){
                        take_idx = i;
                        highestPrio = q->proc[i]->priority;
                }
        }

        struct pcb_t * take_proc = q->proc[take_idx];

        //dich chuyen cac proc sau do ve laij vi tri cu
        for (int i = take_idx+1; i < q->size; i++){
                q->proc[i-1] = q->proc[i];
        }
        q->proc[q->size-1] = NULL;
        q->size--;

        return take_proc;
}

