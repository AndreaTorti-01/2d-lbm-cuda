import sys
import numpy as np
import matplotlib.pyplot as plt
import plotting2D


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python compare.py <reference_binary_file> <output binary file>")
        sys.exit(1)


    ref, tags = plotting2D.read_binary_file(sys.argv[1])
    out, _    = plotting2D.read_binary_file(sys.argv[2])


    if ref.shape != out.shape:
        print("The two files are incompatible")
        sys.exit(1)


    plt.plot(tags, np.max(np.abs(ref - out), axis = (1, 2)))
    plt.xlabel('steps')
    plt.ylabel('maximum difference')
    plt.show()
