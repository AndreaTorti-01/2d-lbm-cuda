NVCC     = nvcc
CC       = g++
CCFLAGS  = -Wall
OPTFLAGS = -O2 -fopenmp
# CONFIG   = -D AB_TESTING

# @TODO: find the default installed version of python
PYTHON   = python3.8


all: lbm serial


lbm: lbm.cu lbm.o
	$(NVCC) $(CONFIG) $^ -o $@


lbm.o: lbm.c lbm.h
	$(CC) -c -o $@ $< $(OPTFLAGS)


serial: main.c lbm.o
	$(CC) $(CCFLAGS) -o $@ $^ -lm


output.bin: serial
	./serial input.txt $@


report: output.bin
	$(PYTHON) scripts/plotting2D.py $^


test: output.bin
	$(PYTHON) scripts/compare.py reference.bin $^
