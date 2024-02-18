NVCC   = nvcc
PYTHON = python3.8


lbm: lbm.cu
	$(NVCC) $^ -o $@


output.bin: lbm
	./lbm $@


report: output.bin
	$(PYTHON) plotting2D.py


test: output.bin
	diff reference.bin $^
