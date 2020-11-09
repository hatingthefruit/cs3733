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
    // Variables to handle different memory parameters.
    unsigned long bytes_per_page, vm_size, pm_size, shift_amt;
    // Vars to manage input/output files
    int inf_fd, of_fd;
    char *inf_name;
    char *of_name;
    // Check the arguments for properness
    if (argc != 6) {
        printf("Usage:\n\t%s infile outfile\n", argv[0]);
        exit(1);
    }

    // Parse arguments to allow for part3/bonus
    bytes_per_page = atoi(argv[1]);
    vm_size = atoi(argv[2]);
    pm_size = atoi(argv[3]);
    // Check that the input arguments are valid
    if (bytes_per_page == 0 || vm_size == 0 || pm_size == 0) {
        printf("Size arguments must be valid non-zero integers\n");
        exit(1);
    }
    inf_name = argv[4];
    of_name = argv[5];

    printf("Bytes: %lu VM: %lu PM: %lu\n", bytes_per_page, vm_size, pm_size);
    // Calculate the masks needed for address calculations
    unsigned long D_MASK = calc_d_mask(bytes_per_page);
    unsigned long V_MASK = calc_v_mask(vm_size, D_MASK);

    // Find the shift amounts for both physical & virtual memory.
    shift_amt = calc_shift(D_MASK);

    pte_t *pte_table = initialize_table(vm_size, bytes_per_page);
    initialize_frames(pm_size, bytes_per_page);

    // Open both input and output files, exit if an error is encountered
    inf_fd = open(inf_name, INF_ARGS);
    if (inf_fd == -1) {
        printf("Error opening %s for reading; error %d\n", inf_name, errno);
        exit(1);
    }
    of_fd = open(of_name, OF_ARGS, OF_MODE);
    if (of_fd == -1) {
        printf("Error opening %s for writing; error %d\n", of_name, errno);
        exit(1);
    }

    // Declare the variables we need to hold our addresses before and after translation, as well as some variables we
    // need for intermediate steps and loop control
    unsigned long vaddr;
    unsigned long paddr;
    char *buff = (char *)&vaddr;
    int write_err;
    // Read until the end of file is reached
    while (read(inf_fd, buff, 8) > 0) {

        paddr = vaddr_to_paddr(vaddr, V_MASK, D_MASK, shift_amt, pte_table);
        if (paddr == ERR_INVALID_PAGE) {
            printf("Page is out of range\n");
        }
        printf("Virtual address %3p -> physical address %3p\n", (void *)vaddr, (void *)paddr);
        write_err = write(of_fd, &paddr, 8);
        if (write_err == -1) {
            printf("Error occured while writing address. Error code: %d\n", errno);
        }
    }

    printf("Number of pagefaults: %u\n", get_page_faults());

    clear_table(pte_table);
    // Close the files
    close(inf_fd);
    close(of_fd);
}
