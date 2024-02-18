NVCC    = nvcc
CC      = gcc
CCFLAGS = -Wall -Werror
PYTHON  = python3.8


all: lbm lbm_serial


lbm: lbm.cu
	$(NVCC) $^ -o $@


lbm_serial: lbm_serial.c
	$(CC) $(CCFLAGS) -o $@ $^


output.bin: lbm
	./lbm $@


report: output.bin
	$(PYTHON) plotting2D.py $^


test: output.bin
	diff reference.bin $^
