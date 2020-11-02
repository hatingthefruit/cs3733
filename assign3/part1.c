#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define INF_ARGS O_RDONLY | O_EXCL
#define OF_ARGS O_CREAT
#define OF_MODE S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP
int main(int argc, char **argv)
{
    int pg_tbl[10] = {2, 4, 1, 7, 3, 5, 6};
    unsigned int bytes_per_page, vm_size, pm_size;
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

    unsigned long *addr = malloc(sizeof(unsigned long));
    char *buff = (char *)addr;
    int read_err;
    do {
        read_err = read(inf_fd, buff, 8);
        // Test that we are reading the input file correctly
        printf("Address read: %p\n", (void *)(*addr));
    } while (read_err != 0);

    // Close the files
    close(inf_fd);
    close(of_fd);
}
