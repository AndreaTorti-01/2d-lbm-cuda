NVCC   = nvcc
PYTHON = python3.8


lbm: lbm.cu
	$(NVCC) $^ -o $@


report:
	./lbm
	$(PYTHON) plotting2D.py
