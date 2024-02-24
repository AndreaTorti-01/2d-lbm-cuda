CC       = gcc
CCFLAGS  = -Wall
OPTFLAGS = -O2 -fopenmp

# @TODO: find the default installed version of python
PYTHON   = python3.8


all: serial


lbm.o: lbm.c lbm.h
	$(CC) -c -o $@ $< $(OPTFLAGS)


serial: main.c lbm.o
	$(CC) $(CCFLAGS) -o $@ $^ -lm


output.bin: serial
	./serial data/input.txt $@


report: output.bin
	$(PYTHON) scripts/plotting2D.py $^


test: output.bin
	$(PYTHON) scripts/compare.py cuda/reference.bin $^


.PHONY folder:
folder:
	mkdir -p build


.PHONY clean:
clean:
	rm -f lbm serial
