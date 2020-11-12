#define NO_FREE_FRAME -1
#define ERR_INVALID_PAGE -2

// Definition for a page table entry. Each hold a frame number, a count for LRU, and a an int (used like a bool) for
// validity.
typedef struct pte {
    unsigned long frame;
    unsigned long count;
    int valid;
} pte_t;

// Functions defined in pagetable.c

// Initialize a pagetable. This function malloc's memory, which needs to be freed using clear_table() after use.
// Arguments:
//  unsigned long vm_size: A power of two specifying the size of the virtual memory
//  unsigned long bytes_per_page: A power of two specifying the number of bytes addressed per page
// Return:
//  A pte_t* that points to the first element of an array of pte_t elements. This array is malloc'd, and needs to be
//  freed after use.
pte_t *initialize_table(unsigned long vm_size, unsigned long bytes_per_page);
// Calculate the mask needed to isolate offset bits
// Arguments:
//  unsigned long bytes_per_page: A power of two specifying the size of each page/frame of memory
// Returns:
//  A mask that can be used to isolate only the offset bits of a virtual address
unsigned long calc_d_mask(unsigned long bytes_per_page);
// Calculate the mask needed to isolate the page number
// Arguments:
//  unsigned long vm_size: A power of two specifying the size of the virtual memory address space
//  unsigned long d_mask: The mask for the offset bits
// Returns:
//  A mask that can be used to isolate only the page bits of a virtual address
unsigned long calc_v_mask(unsigned long vm_size, unsigned long d_mask);
// Calculate the shift needed to translate page bits into a page number
// Arguments:
//  unsigned long d_mask: The mask for offset bits
// Return:
//  The number of shifts needed to translate page bits into a page number, or frame numbers into frame bits
unsigned long calc_shift(unsigned long d_mask);
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
                             pte_t *table);
// Free the memory used for the page table
// Arguments:
//  None
// Return:
//  None
void clear_table(pte_t *table);
// Get the number of page faults encountered
// Arguments:
//  None
// Return:
//  The number of page faults encountered so far
unsigned int get_page_faults();

// Functions defined in phypages.c

// Initialize the frames
// Arguments:
//  unsigned long pm_size: Total size of the physical memory
//  unsigned long bytes_per_page: The number of bytes in each page/frame
// Return:
//  None
void initialize_frames(unsigned long pm_size, unsigned long bytes_per_page);
// Get the next free frame.
// Arguments:
//  None
// Return:
//  The next free frame if one is available, or NO_FREE_FRAME if no free frame is available
unsigned long get_free_frame();
