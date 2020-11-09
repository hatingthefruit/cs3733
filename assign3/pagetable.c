#include "pages.h"
#include <limits.h>
#include <stdlib.h>

unsigned int page_faults;
unsigned long num_pte;
unsigned long lru_count;

pte_t *initialize_table(unsigned long vm_size, unsigned long bytes_per_page)
{
    int i;
    num_pte = vm_size / bytes_per_page;
    pte_t *table = malloc(sizeof(pte_t) * num_pte);
    for (i = 0; i < num_pte; i++) {
        table[i].valid = 0;
        table[i].count = ULONG_MAX;
    }
    page_faults = 0;
    lru_count = 0;
    return table;
}

unsigned long calc_d_mask(unsigned long bytes_per_page)
{
    return (bytes_per_page - 1);
}
unsigned long calc_v_mask(unsigned long vm_size, unsigned long d_mask)
{
    return ((vm_size - 1) ^ d_mask);
}

// Find the shift amounts for both physical & virtual memory. This is essentially the number of bits taken up by
// the offset. In order to lookup a frame number, we first have to convert the page bits into a normalized page
// number. As an example: Virtual address: yyyyyXXXXXXX The page number is represented by yyyyy, which needs to
// be converted from yyyyy0000000 to just yyyyy to be used as a regular number. This can be done by shifting
// right by the number of bits used by the offset.Once a frame number is looked up, the frame number similarly
// needs to be shifted left the same number of bits in order to be added to the offset.
unsigned long calc_shift(unsigned long d_mask)
{
    int i;
    unsigned long shift_amt = 0;
    for (i = d_mask; i != 0; i = i >> 1) {
        shift_amt++;
    }
    return shift_amt;
}

unsigned long vaddr_to_paddr(unsigned long vaddr, unsigned long v_mask, unsigned long d_mask, unsigned long shift,
                             pte_t *table)
{
    unsigned long pnum, paddr, i;
    // Find the page number by selecting only the page bits and then shifting right
    pnum = ((vaddr)&v_mask) >> shift;
    // Create the physical address by looking up the frame number, shifting it left, and then adding the offset bits
    // back in
    /*if (pnum >= num_pte) {
        return ERR_INVALID_PAGE;
    }*/
    if (table[pnum].valid == 0) {
        page_faults++;
        unsigned long new_frame = get_free_frame();

        if (new_frame == NO_FREE_FRAME) {
            pte_t *min = &(table[0]);
            for (i = 1; i < num_pte; i++) {
                if (table[i].valid != 0 && table[i].count < min->count) {
                    min = &(table[i]);
                }
            }
            min->valid = 0;
            min->count = ULONG_MAX - 1;
            new_frame = min->frame;
        }
        table[pnum].frame = new_frame;
        table[pnum].valid = 1;
    }

    table[pnum].count = lru_count;
    lru_count++;
    paddr = (table[pnum].frame << shift) | ((vaddr)&d_mask);
    return paddr;
}

void clear_table(pte_t *table)
{
    free(table);
};
unsigned int get_page_faults()
{
    return page_faults;
}