import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def mandelbulb(z, max_iter, power):
    c = z
    for n in range(max_iter):
        if abs(z).any() > 2.0:
            return n
        r = np.sqrt((z.real ** 2) + (z.imag ** 2) + (z.imag ** 2))
        theta = np.arccos(z.real / r)
        phi = np.arctan2(z.imag, z.real)
        z = r ** power * np.exp(1j * power * theta) * np.cos(power * phi) + c
    return max_iter

def generate_mandelbulb(width, height, max_iter, power):
    x = np.linspace(-2, 2, width)
    y = np.linspace(-2, 2, height)
    X, Y = np.meshgrid(x, y)
    Z = X + 1j * Y
    mandelbulb_set = np.zeros_like(Z, dtype=int)
    for i in range(width):
        for j in range(height):
            mandelbulb_set[i, j] = mandelbulb(Z[i, j], max_iter, power)
    return mandelbulb_set

def plot_mandelbulb(mandelbulb_set):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    width, height = mandelbulb_set.shape
    for i in range(width):
        for j in range(height):
            ax.scatter(i, j, mandelbulb_set[i, j], color='b', s=1)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()

if __name__ == "__main__":
    width = 400
    height = 400
    max_iter = 100
    power = 8
    mandelbulb_set = generate_mandelbulb(width, height, max_iter, power)
    plot_mandelbulb(mandelbulb_set)