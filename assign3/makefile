CC=gcc
CCARGS=-Wall -g
OBJS=pagetable.o phypages.o
INCLUDES=pages.h
TARGETS=part1 part2 part3

all: $(TARGETS)

$(TARGETS) : % : %.c $(OBJS)
	$(CC) $(CCARGS) $< -o $@ $(OBJS)

$(OBJS) : %.o : %.c $(INCLUDES)
	gcc $(CCARGS) -c $< -o $@

clean:
	rm -f $(TARGETS)
	rm -f $(OBJS)