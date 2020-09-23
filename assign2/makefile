CC=gcc
objects = assign2.o pslibrary.o
includes = pslibrary.h
initTARGET = assign2
endTARGET = runall

# Default Rules for building each .o
%.o: %.c ${includes}
	gcc -g -c $<

# Rule for building the executable
all: ${objects}
	gcc -g -o bin/${initTARGET} ${objects}

# Special rule to remove the .o files
clean:
	rm -f ${objects} bin/${initTARGET} bin/${endTARGET}
