CC=gcc
CCARGS=-Wall -g
objects = assign2.o pslibrary.o
includes = pslibrary.h
initTARGET = assign2
endTARGET = runall

# Default Rules for building each .o
%.o: %.c ${includes}
	gcc ${CCARGS} -c $<

# Rule for building the executable
all: ${objects}
	gcc ${CCARGS} -o ${initTARGET} ${objects}

# Rule for running all test cases
testassign: all
	./${initTARGET} 3 8 7 3 6 3 2
	./${initTARGET} 3 8 7 3 6 7 2
	./${initTARGET} 4 8 7 3 6 1 2
	./${initTARGET} 3 3 3 4 2 1 2
	./${initTARGET} 3 3 2 3 2 1 2
	./${initTARGET} 3 5 2 5 4 1 1
	./${initTARGET} 3 2 3 4 1 1 5
	./${initTARGET} 3 2 3 4 1 4 1
	./${initTARGET} 3 2 3 4 5 6 2

# Rule for running the final program
testbuild: ${objects} ${endTARGET}.c
	gcc ${CCARGS} -o ${endTARGET} ${endTARGET}.c pslibrary.o

testall: ${objects} ${endTARGET}.c testbuild
	./${endTARGET}
	ls -l *.all
	wc *.all
	md5sum *.all
	rm *.all

# Special rule to remove the .o files
clean:
	rm -f ${objects} ${initTARGET} ${endTARGET}
