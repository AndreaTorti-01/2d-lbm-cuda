# Lattice Boltzmann Method CFD CUDA Implementation

This project is derived from https://github.com/AndreaTorti-01/Lattice-Boltzmann-Method, refer to that for more information

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
