#include "pages.h"
#include <stdio.h>

// Globals because they make sense here
unsigned long num_frames;      // Total number of frames, calculated from size of memory and bytes per page
unsigned long next_free_frame; // Counter for the next free frame

// Initialize the frames
// Arguments:
//  unsigned long pm_size: Total size of the physical memory
//  unsigned long bytes_per_page: The number of bytes in each page/frame
// Return:
//  None
void initialize_frames(unsigned long pm_size, unsigned long bytes_per_page)
{
    num_frames = pm_size / bytes_per_page;
    next_free_frame = 1;
}

// Get the next free frame.
// Arguments:
//  None
// Return:
//  The next free frame if one is available, or NO_FREE_FRAME if no free frame is available
unsigned long get_free_frame()
{
    // Save the next free frame
    unsigned long old_frame = next_free_frame;

    // If there is a free frame...
    if (next_free_frame < num_frames) {
        //... increment the free frame counter and return the old value of it
        next_free_frame++;
        return old_frame;
    }
    else {
        // Otherwise, return the NO_FREE_FRAME flag
        return NO_FREE_FRAME;
    }
}