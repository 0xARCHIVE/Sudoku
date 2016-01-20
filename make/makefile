#to run, call mingw32-make
#compiler flags
CFLAGS = -I.

#libraries
libs = 

#source and output settings
src_dir = ../src
out_dir = ../output
out_name = sudoku

objects = main.o sudoku.o

$(out_dir)/$(out_name) : $(objects)
	cc -o $(out_dir)/$(out_name) $(objects) $(libs)

#compiles all objects.c into objects.o
$(objects): %.o: $(src_dir)/%.c
	cc -c $(CFLAGS) $< -o $@