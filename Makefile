NVCC    = nvcc
CC      = g++
CCFLAGS = -Wall -Werror
PYTHON  = python3.8

CONFIG = -D AB_TESTING


all: lbm lbm_serial


lbm: lbm.cu lbm.o
	$(NVCC) $(CONFIG) $^ -o $@


lbm.o: lbm.c
	$(CC) -c -o $@ $^


lbm_serial: lbm_serial.c
	$(CC) $(CCFLAGS) -o $@ $^


output.bin: lbm
	./lbm $@


report: output.bin
	$(PYTHON) plotting2D.py $^


test: output.bin
	diff reference.bin $^
