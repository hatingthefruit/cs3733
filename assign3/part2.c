#include "pages.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define INF_ARGS O_RDONLY | O_EXCL
#define OF_ARGS O_CREAT | O_WRONLY
#define OF_MODE S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP

int main(int argc, char **argv)
{
    // Variables to handle different memory parameters. Need to track physical memory size in this part, unlike part1
    unsigned long bytes_per_page, vm_size, pm_size, shift_amt;
    // Vars to manage input/output files
    FILE *inf_fd, *of_fd;
    char *inf_name;
    char *of_name;
    // Check the arguments for properness
    if (argc != 3) {
        printf("Usage:\n\t%s infile outfile\n", argv[0]);
        exit(1);
    }

    // Set things to defaults
    bytes_per_page = 128;
    vm_size = 4096;
    pm_size = 1024;

    // pull file names from arguments
    inf_name = argv[1];
    of_name = argv[2];

    // Open both input and output files, exit if an error is encountered
    inf_fd = fopen(inf_name, "rb");
    if (inf_fd == NULL) {
        printf("Error opening %s for reading; error %d\n", inf_name, errno);
        exit(1);
    }
    of_fd = fopen(of_name, "wb");
    if (of_fd == NULL) {
        printf("Error opening %s for writing; error %d\n", of_name, errno);
        exit(1);
    }

    // Calculate the masks needed for address calculations
    unsigned long D_MASK = calc_d_mask(bytes_per_page);
    unsigned long V_MASK = calc_v_mask(vm_size, D_MASK);

    // Find the shift amounts for both physical & virtual memory.
    shift_amt = calc_shift(D_MASK);

    // Initialize the page table and the physical frames
    pte_t *pte_table = initialize_table(vm_size, bytes_per_page);
    initialize_frames(pm_size, bytes_per_page);

    // Declare the variables we need to hold our addresses before and after translation, as well as some variables we
    // need for intermediate steps and loop control
    unsigned long vaddr;
    unsigned long paddr;
    char *buff = (char *)&vaddr;
    int write_err;
    // Read until the end of file is reached
    while (fread(buff, 8, 1, inf_fd) > 0) {
        // In part 1, address translation logic was hardcoded here. Now translation is performed by calling a library
        // function from pagetable.c
        paddr = vaddr_to_paddr(vaddr, V_MASK, D_MASK, shift_amt, pte_table);
        // Check to make sure the page we passed was a valid one
        if (paddr == ERR_INVALID_PAGE) {
            printf("Page is out of range\n");
            continue;
        }
        // Otherwise, write the address translation to the output file
        write_err = fwrite(&paddr, 8, 1, of_fd);
        if (write_err != 8 && ferror(of_fd)) {
            printf("Error occured while writing address. Error code: %d\n", errno);
            exit(1);
        }
    }
    // Print the number of pagefaults encountered
    printf("Number of pagefaults: %u\n", get_page_faults());

    // The page table is malloc'd in the initialize function. Call this to free the memory.
    clear_table(pte_table);
    // Close the files
    fclose(inf_fd);
    fclose(of_fd);
}
