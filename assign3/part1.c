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
    // Hardcoded pagetable
    int pg_tbl[10] = {2, 4, 1, 7, 3, 5, 6};
    // Variables to handle different memory parameters
    unsigned long bytes_per_page, vm_size, pm_size, shift_amt, i;
    // Vars to manage input/output files
    int inf_fd, of_fd;
    char *inf_name;
    char *of_name;
    // Check the arguments for properness
    if (argc != 3 && argc != 6) {
        printf("Usage:\n\t%s infile outfile\n", argv[0]);
        exit(1);
    }
    else if (argc == 6) {
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
    }
    else {
        // Set things to defaults if parameters for the run are not specified
        bytes_per_page = 128;
        vm_size = 4096;
        pm_size = 1024;

        inf_name = argv[1];
        of_name = argv[2];
    }

    // Calculate the masks needed for address calculations
    unsigned long D_MASK = (bytes_per_page - 1);
    unsigned long V_MASK = ((vm_size - 1) ^ D_MASK);

    // Find the shift amounts for both physical & virtual memory. This is essentially the number of bits taken up by
    // the offset. In order to lookup a frame number, we first have to convert the page bits into a normalized page
    // number. As an example: Virtual address: yyyyyXXXXXXX The page number is represented by yyyyy, which needs to
    // be converted from yyyyy0000000 to just yyyyy to be used as a regular number. This can be done by shifting
    // right by the number of bits used by the offset.Once a frame number is looked up, the frame number similarly
    // needs to be shifted left the same number of bits in order to be added to the offset.
    shift_amt = 0;
    for (i = D_MASK; i != 0; i = i >> 1) {
        shift_amt++;
    }

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
    unsigned long paddr, pnum;
    char *buff = (char *)&vaddr;
    int write_err;
    // Read until the end of file is reached
    while (read(inf_fd, buff, 8) > 0) {
        // Find the page number by selecting only the page bits and then shifting right
        pnum = ((vaddr)&V_MASK) >> shift_amt;
        // Create the physical address by looking up the frame number, shifting it left, and then adding the offset bits
        // back in
        paddr = (pg_tbl[pnum] << shift_amt) + ((vaddr)&D_MASK);
        // Print the address translation, then write the translated address to the output file
        printf("Virtual address %3p -> physical address %3p\n", (void *)vaddr, (void *)paddr);
        write_err = write(of_fd, &paddr, 8);
        if (write_err == -1) {
            printf("Error occured while writing address. Error code: %d\n", errno);
        }
    }

    // Close the files
    close(inf_fd);
    close(of_fd);
}
