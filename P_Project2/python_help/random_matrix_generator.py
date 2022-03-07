import numpy as np

if __name__ == '__main__':
    for size in [64, 128, 512, 1024]:
        pathA = "../data/mat-A-%d.txt" % size
        pathB = "../data/mat-B-%d.txt" % size
        matrixA = np.random.rand(size, size) * 100
        matrixB = np.random.rand(size, size) * 100
        np.savetxt(pathA, matrixA, fmt='%.1f', delimiter=" ")
        np.savetxt(pathB, matrixB, fmt='%.1f', delimiter=" ")
