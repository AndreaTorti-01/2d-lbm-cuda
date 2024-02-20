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
 3. Write equivalent serial implementation
 4. Make the serial implementation more readable
 5. Restore (and maybe refactor) the CUDA implementation to its initial state
 6. Understand the numerical difference between serial and CUDA implementation
 7. Think about the rendering part


## Usage
`make report` for computing and visualizing the lbm experiment
`make test` for comparing the serial and CUDA implementations


## Output file format
Plain text and binary format:

```
width height
iteration_count
[width * height fp32 values]
iteration_count
[width * height fp32 values]
...
```

Since the floating point values represent the state in a 2D matrix, we have to care about the row-major / column-major (TODO)


## Discrepancy between serial and CUDA version
From the A/B testing I found that the `step1` function yields different results for CUDA and serial versions, but I have an hypothesis: this particular function is full of floating point computation and there are many constants (like `1.0` for example). The C compiler promotes the fp32 values to fp64 if an operand of the expression is a fp64, and this is the case for the constants because they are by default fp64. The CUDA compiler might not use fp64 intermediate values since its standard precision is fp32.

There are two possible choices:
 1. Cast all the numeric constants to fp32 in CUDA kernels and reproduce `reference.bin`
 2. Compare better the output files (using the norm)

Seeing the results of the serial version in python, I'm much more confident that this is only a precision problem.
