# 2d-lbm-cuda


## Why this fork
This fork is an exercise in software development:
 * reading source code
 * understanding the design choices of @AndreaTorti-01
 * writing a CPU version of CUDA code
 * (explore openMP optimizations)
 * render the experiment results in real time with openGL
 * porting the CUDA code to openGL shaders (or openCL)
 * tinker the parameters of the simulation in real time


## Steps
 1. Makefile
 2. Producing reference output on a CUDA machine (google colab environment)


## Usage

Windows:

```powershell
nvcc lbm.cu -o lbm
python -m venv env
.\env\Scripts\Activate.ps1
pip install -r requirements.txt
.\run.ps1
```

Linux:

```bash
nvcc lbm.cu -o lbm
python3 -m venv env
source env/bin/activate
pip install -r requirements.txt
./run.sh
```
