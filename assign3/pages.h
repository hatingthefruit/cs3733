#define NO_FREE_FRAME -1
#define ERR_INVALID_PAGE -2

typedef struct pte {
    unsigned long frame;
    unsigned long count;
    int valid;
} pte_t;

// Functions defined in pagetable.c
pte_t *initialize_table(unsigned long vm_size, unsigned long bytes_per_page);
unsigned long calc_d_mask(unsigned long bytes_per_page);
unsigned long calc_v_mask(unsigned long vm_size, unsigned long d_mask);
unsigned long calc_shift(unsigned long d_mask);
unsigned long vaddr_to_paddr(unsigned long vaddr, unsigned long v_mask, unsigned long d_mask, unsigned long shift,
                             pte_t *table);
void clear_table(pte_t *table);
unsigned int get_page_faults();

// Functions defined in phypages.c
void initialize_frames(unsigned long pm_size, unsigned long bytes_per_page);
unsigned long get_free_frame();
