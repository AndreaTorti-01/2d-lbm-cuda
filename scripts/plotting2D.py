import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time


def read_binary_file(filename):
    SIZEOF_FLOAT32 = np.dtype(np.float32).itemsize

    frames = []
    tags   = []

    with open(filename, 'rb') as f:
        width, height = [int(x) for x in f.readline().split()]

        line = f.readline()
        while line:
            tags.append(int(line))

            buffer = f.read(SIZEOF_FLOAT32 * width * height)
            frames.append(np.frombuffer(buffer, dtype = np.float32))

            line = f.readline()

        U = np.array(frames).reshape(-1, height, width)

        return (U, tags)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python plotting2D.py <binary_file_path>")
        sys.exit(1)


    filename = sys.argv[1]

    
    # create numpy tridimensional array to store velocity moduluses
    all_U, all_Steps = read_binary_file(filename)
    
    # Create the figure and the initial plot
    fig, ax = plt.subplots()
    im = ax.imshow(all_U[0], origin='upper', cmap='viridis', vmin=0, vmax=0.3, interpolation='lanczos')
    # cbar = fig.colorbar(im)
    title = ax.set_title(f'Step {all_Steps[0]}')
    plt.tight_layout()
    
    # Function to update the plot for each frame
    def update(frame):
        im.set_array(all_U[frame])
        title.set_text(f'Step {all_Steps[frame]}')
        print(f'Created {frame}')
    
    # Create the animation
    animation = FuncAnimation(fig, update, frames=len(all_U), interval=100, repeat=False)
    
    # Save the animation as video with system current time in the name
    timestr = time.strftime("%Y%m%d-%H%M%S")
    plt.show()
