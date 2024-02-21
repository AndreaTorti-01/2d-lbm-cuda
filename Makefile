NVCC     = nvcc
CC       = g++
CCFLAGS  = -Wall
OPTFLAGS = -O2 -fopenmp
PYTHON   = python3.8
CONFIG   = -D AB_TESTING


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
	$(PYTHON) plotting2D.py $^


test: output.bin
	diff reference.bin $^
