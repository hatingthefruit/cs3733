#include "pages.h"
#include <stdio.h>

unsigned long num_frames;
unsigned long next_free_frame;

void initialize_frames(unsigned long pm_size, unsigned long bytes_per_page)
{
    num_frames = pm_size / bytes_per_page;
    next_free_frame = 1;
}

unsigned long get_free_frame()
{
    unsigned long old_frame = next_free_frame;
    if (next_free_frame < num_frames) {
        next_free_frame++;
        return old_frame;
    }
    else {
        return NO_FREE_FRAME;
    }
}