// #ifdef MM_PAGING
/*
 * PAGING based Memory Management
 * Virtual memory module mm/mm-vm.c
 */

#include "string.h"
#include "mm.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/*get_vma_by_num - get vm area by numID
 *@mm: memory region
 *@vmaid: ID vm area to alloc memory region
 *
 */
struct vm_area_struct *get_vma_by_num(struct mm_struct *mm, int vmaid)
{
  struct vm_area_struct *pvma = mm->mmap;

  if (mm->mmap == NULL)
    return NULL;

  int vmait = pvma->vm_id;

  while (vmait < vmaid)
  {
    if (pvma == NULL)
      return NULL;

    pvma = pvma->vm_next;
    vmait = pvma->vm_id;
  }

  return pvma;
}

int __mm_swap_page(struct pcb_t *caller, int vicfpn , int swpfpn)
{
    __swap_cp_page(caller->mram, vicfpn, caller->active_mswp, swpfpn);
    return 0;
}

/*get_vm_area_node - get vm area for a number of pages
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
struct vm_rg_struct *get_vm_area_node_at_brk(struct pcb_t *caller, int vmaid, int size, int alignedsz)
{
  struct vm_rg_struct * newrg;
  /* TODO retrive current vma to obtain newrg, current comment out due to compiler redundant warning*/
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  if (cur_vma == NULL)  return NULL;         

  newrg = malloc(sizeof(struct vm_rg_struct));
  if (newrg == NULL) return NULL;

  // TODO: update the newrg boundary
  newrg->rg_start = (cur_vma->sbrk+alignedsz-1) / alignedsz * alignedsz;
  // co the su dung alignnedsz thay cho size ?
  newrg->rg_end = newrg->rg_start + size;

  
  return newrg;
}

/*validate_overlap_vm_area
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
int validate_overlap_vm_area(struct pcb_t *caller, int vmaid, int vmastart, int vmaend)
{
  struct vm_area_struct *vma = caller->mm->mmap;
  // struct vm_area_struct *tmp = vma;

  if (vma == NULL) return -1;
  /* TODO validate the planned memory area is not overlapped */

  while (vma != NULL)
  {
    if (vmaid == vma->vm_id)
    {
      vma = vma->vm_next;
      continue; //bo qua vung dang muon mo rong bang vmaid
    }
    if ((vmastart >= vma->vm_start && vmastart < vma->vm_end) ||
      (vmaend > vma->vm_start && vmaend <= vma->vm_end) ||
      (vmastart <= vma->vm_start && vmaend >= vma->vm_end))
      return -1; // return loi vi overlap 
    
    vma = vma->vm_next;
  }

  // while(tmp != NULL){
  //   if (tmp->vm_id == vmaid){
  //     tmp->vm_start = vmastart;
  //     tmp->vm_end = vmaend;
  //   }
  //   tmp = tmp->vm_next;
  // }
  return 0;
}

/*inc_vma_limit - increase vm area limits to reserve space for new variable
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@inc_sz: increment size
 *
 */
int inc_vma_limit(struct pcb_t *caller, int vmaid, int inc_sz)
{
  struct vm_rg_struct * newrg = malloc(sizeof(struct vm_rg_struct));
  if (newrg == NULL) return -1;

  int inc_amt = PAGING_PAGE_ALIGNSZ(inc_sz);
  int incnumpage =  inc_amt / PAGING_PAGESZ;

  struct vm_rg_struct *area = get_vm_area_node_at_brk(caller, vmaid, inc_sz, inc_amt);
  if (area == NULL) return -1;

  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  if (cur_vma == NULL) return -1;

  int old_end = cur_vma->vm_end;

  /*Validate overlap of obtained region */

  if (validate_overlap_vm_area(caller, vmaid, area->rg_start, area->rg_end) < 0)
    return -1; /*Overlap and failed allocation */

  /* TODO: Obtain the new vm area based on vmaid */

  // cur_vma->vm_end += area->rg_end;
  cur_vma->vm_end += inc_sz;
  cur_vma->sbrk = cur_vma->vm_end;
  // inc_limit_ret...
  if (vm_map_ram(caller, area->rg_start, area->rg_end, 
                    old_end, incnumpage , newrg) < 0){
                    cur_vma->vm_end = old_end;
    return -1; /* Map the memory to MEMRAM */
  }
  return 0;
}

// #endif
