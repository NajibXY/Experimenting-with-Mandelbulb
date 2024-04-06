import multiprocessing as mp
import matplotlib.pyplot as plt
import matplotlib.colors as pltcolors
import numpy as np
import os
from pylab import cm

def plot_density(iteration, datafolder, distance_file, light_file):
    # Dimensions of the image
    xratio = 3.0
    yratio = 3.0
    # Distance data
    data_distance = np.genfromtxt(datafolder+distance_file, delimiter=',')
    # Light data
    data_light = np.genfromtxt(datafolder+light_file, delimiter=',')
    # Combine data
    data = data_distance + data_light

    ## Save plotted data

    # Figure
    fig = plt.figure(frameon=False)
    np.random.seed(0)  # Add a seed value to the random generator
    fig.set_size_inches(xratio, yratio)
    ax = plt.Axes(fig, [0.0, 0.0, 1.0, 1.0], )
    ax.set_axis_off()
    fig.add_axes(ax)
    # Plot distance data
    cmap = pltcolors.ListedColormap(np.random.seed(iteration).default_rng().random((256, 3)))
    plt.imshow(data_distance, origin='lower', extent=[0, xratio, 0, yratio], cmap=cmap)
    # Save figure
    plt.savefig('images/distance_'+str(iteration).zfill(3)+'.png', dpi=300)
    # Plot light data
    plt.imshow(data_light, origin='lower', extent=[0, xratio, 0, yratio], cmap=cmap)
    # Save figure
    plt.savefig('images/light_'+str(iteration).zfill(3)+'.png', dpi=300)
    # Plot merged data
    plt.imshow(data, origin='lower', extent=[0, xratio, 0, yratio], cmap=cmap)
    plt.savefig('images/mandelbulb_'+str(iteration).zfill(3)+'.png', dpi=300)
    # Close figure
    plt.close()

if __name__ == '__main__':
    plt.close('all')
    datafolder = './data/'
    distance_filepaths  = sorted([filename for filename in os.listdir(datafolder) if filename.startswith("distance_")])
    light_filepaths = sorted([filename for filename in os.listdir(datafolder) if filename.startswith("light_")])
    distance_filepaths  = distance_filepaths[::3]
    light_filepaths = light_filepaths[::3]
    # Rendering images in parallel
    pool = mp.Pool()
    results = [pool.apply_async(
                        plot_density, 
                        args=(iteration, datafolder, distance_filepaths[iteration], light_filepaths[iteration],)
                    )
                for iteration in range(len(min(distance_filepaths, light_filepaths)))]
    results = [p.get() for p in results]
    # Generating video from images through ffmpeg at 1440p resolution and 60 fps
    os.system('ffmpeg -start_number 0 -i .\images\mandelbulb_%03d.png -vb 40M -framerate 60 -vf scale=1440:-1 -vcodec mpeg4 mandelbulb.mp4')