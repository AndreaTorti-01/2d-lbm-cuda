import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time
import struct

filename = "output.bin"

# create numpy tridimensional array to store velocity moduluses
all_U = []
all_Steps = []

with open(filename, 'rb') as f:
    # read two integers: width and height of lattice
    line = f.readline()
    width, height = [int(x) for x in line.split()]
    length = width * height
    
    i = 0
    # while line is not empty
    line = f.readline()
    while line:
        step = int(line)  # convert iteration number to integer
        # read length Ux floats: x-velocities
        U = []
        for _ in range(length):
            data = f.read(4)  # 4 bytes for a float
            if len(data) != 4:
                break
            float_value = struct.unpack('f', data)[0]
            U.append(float_value)
        
        all_U.append(U)
        all_Steps.append(step)
        print(f'Read {i}')
        i += 1        
        line = f.readline()

# Reshape the 1D list into a 2D array
all_U = np.array(all_U).reshape(-1, height, width)

# Create the figure and the initial plot
fig, ax = plt.subplots()
im = ax.imshow(all_U[0], origin='upper', cmap='plasma', vmin=0, vmax=0.3, interpolation='lanczos')
cbar = fig.colorbar(im)
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