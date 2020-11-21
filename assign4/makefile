CC=gcc
CCARGS=-Wall -g -pthread
INCLUDES=pages.h
TARGETS=assign4-part1 assign4-part2 assign4-part3

all: $(TARGETS)

$(TARGETS) : % : %.c 
	$(CC) $(CCARGS) $< -o $@ 


clean:
	rm -f $(TARGETS)
	rm -f $(OBJS)