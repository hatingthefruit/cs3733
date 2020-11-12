#include "pages.h"
#include <limits.h>
#include <stdlib.h>

// Globals, because they make sense here.
unsigned int page_faults; // Counter for page faults
unsigned long num_pte;    // Total size of the page table in terms of entries
unsigned long lru_count;  // Counter for keeping track of LRU status

// Initialize a pagetable. This function malloc's memory, which needs to be freed using clear_table() after use.
// Arguments:
//  unsigned long vm_size: A power of two specifying the size of the virtual memory
//  unsigned long bytes_per_page: A power of two specifying the number of bytes addressed per page
// Return:
//  A pte_t* that points to the first element of an array of pte_t elements. This array is malloc'd, and needs to be
//  freed after use.
pte_t *initialize_table(unsigned long vm_size, unsigned long bytes_per_page)
{
    // iterator
    int i;
    // Set the total number of page table entries
    num_pte = vm_size / bytes_per_page;

    // Allocate a page table
    pte_t *table = malloc(sizeof(pte_t) * num_pte);

    // Initialize all valid bits to zero, and all counters to the max so no invalid page is considered LRU
    for (i = 0; i < num_pte; i++) {
        table[i].valid = 0;
        table[i].count = ULONG_MAX;
    }

    // Set counters to 0 and return the table
    page_faults = 0;
    lru_count = 0;
    return table;
}

// Calculate the mask needed to isolate offset bits
// Arguments:
//  unsigned long bytes_per_page: A power of two specifying the size of each page/frame of memory
// Returns:
//  A mask that can be used to isolate only the offset bits of a virtual address
unsigned long calc_d_mask(unsigned long bytes_per_page)
{
    // Subtract 1 from a power of two to get all 1s in the bits needed to represent that number of values
    return (bytes_per_page - 1);
}

// Calculate the mask needed to isolate the page number
// Arguments:
//  unsigned long vm_size: A power of two specifying the size of the virtual memory address space
//  unsigned long d_mask: The mask for the offset bits
// Returns:
//  A mask that can be used to isolate only the page bits of a virtual address
unsigned long calc_v_mask(unsigned long vm_size, unsigned long d_mask)
{
    // Similar to calculating the offset mask, but xor the result with the offset mask to only leave the page bits
    return ((vm_size - 1) ^ d_mask);
}

// Calculate the shift needed to translate page bits into a page number
// Arguments:
//  unsigned long d_mask: The mask for offset bits
// Return:
//  The number of shifts needed to translate page bits into a page number, or frame numbers into frame bits
unsigned long calc_shift(unsigned long d_mask)
{
    int i;
    unsigned long shift_amt = 0;

    // Find the shift amounts for both physical & virtual memory. This is essentially the number of bits taken up by
    // the offset. In order to lookup a frame number, we first have to convert the page bits into a normalized page
    // number. As an example: Virtual address: yyyyyXXXXXXX The page number is represented by yyyyy, which needs to
    // be converted from yyyyy0000000 to just yyyyy to be used as a regular number. This can be done by shifting
    // right by the number of bits used by the offset.Once a frame number is looked up, the frame number similarly
    // needs to be shifted left the same number of bits in order to be added to the offset.
    for (i = d_mask; i != 0; i = i >> 1) {
        shift_amt++;
    }
    return shift_amt;
}

// Perform address translation from virtual to physical addresses
// Arguments:
//  unsigned long vaddr: The virtual address to be translated
//  unsigned long v_mask: The mask used for isolating page bits
//  unsigned long d_mask: The mask used for isolation offset bits
//  unsigned long shift: The shift needed to translate page or frame bits into numbers or vice-versa
//  pte_t *table: A pointer to a page table, which is just an array of pte_t initialized by initialize_table()
// Return:
//  The physical address translation from the virtual address passed, or ERR_INVALID_PAGE if the vaddr passed is out of
//  range
unsigned long vaddr_to_paddr(unsigned long vaddr, unsigned long v_mask, unsigned long d_mask, unsigned long shift,
                             pte_t *table)
{
    unsigned long pnum, paddr, i;
    // Find the page number by selecting only the page bits and then shifting right
    pnum = ((vaddr)&v_mask) >> shift;
    // Create the physical address by looking up the frame number, shifting it left, and then adding the offset bits
    // back in
    if (pnum >= num_pte) {
        return ERR_INVALID_PAGE;
    }

    // If the page requested isn't valid, make it valid
    if (table[pnum].valid == 0) {
        // Increment page faults
        page_faults++;
        // Get a free frame for the now invalid page
        unsigned long new_frame = get_free_frame();

        // If there are no free frames left, steal from the LRU page
        if (new_frame == NO_FREE_FRAME) {
            // Find the pte with the minimum count -> LRU page
            pte_t *min = &(table[0]);
            for (i = 1; i < num_pte; i++) {
                if (table[i].valid != 0 && table[i].count < min->count) {
                    min = &(table[i]);
                }
            }

            // Invalidate the old frame and set the count to max so it is no longer considered for LRU
            min->valid = 0;
            min->count = ULONG_MAX;
            new_frame = min->frame;
        }
        // Set the frame for the page to either the new frame, or the one stolen from another page. Set the page to
        // valid
        table[pnum].frame = new_frame;
        table[pnum].valid = 1;
    }

    // Update the count for the accessed page
    table[pnum].count = lru_count;
    lru_count++;

    // Calculate the physical address to return
    paddr = (table[pnum].frame << shift) | ((vaddr)&d_mask);
    return paddr;
}

// Free the memory used for the page table. This is basically just a wrapper for free()
// Arguments:
//  None
// Return:
//  None
void clear_table(pte_t *table)
{
    free(table);
}

// Free the memory used for the page table
// Arguments:
//  None
// Return:
//  None
unsigned int get_page_faults()
{
    return page_faults;
}