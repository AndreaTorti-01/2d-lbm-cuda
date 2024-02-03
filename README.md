# 2d-lbm-cuda

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
