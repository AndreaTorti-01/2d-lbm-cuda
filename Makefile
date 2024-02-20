NVCC    = nvcc
CC      = g++
CCFLAGS = -Wall
PYTHON  = python3.8

CONFIG = -D AB_TESTING


all: lbm serial


lbm: lbm.cu lbm.o
	$(NVCC) $(CONFIG) $^ -o $@


lbm.o: lbm.c
	$(CC) -c -o $@ $^ -O2


serial: main.c lbm.o
	$(CC) $(CCFLAGS) -o $@ $^ -lm


output.bin: serial
	./serial $@


report: output.bin
	$(PYTHON) plotting2D.py $^


test: output.bin
	diff reference.bin $^
