# import csv
# import pandas as pd
import numpy as np
# from decimal import Decimal
if __name__ == '__main__':
    # size = 2048
    # type = Decimal
    # type = np.typeDict['f']
    for size in [32, 64, 128,256, 512, 1024, 2048]:
        type = np.typeDict['f']

        pathA = "../resources/input_data/mat-A-%d.txt" % size
        pathB = "../resources/input_data/mat-B-%d.txt" % size
        pathC = "../resources/output_data/mat-C-{0:d}_by_python_{1}.txt".format(size, "float" if type == np.typeDict['f'] else "double")
        # matrixA = pd.DataFrame(matrixAFile).to_numpy()
        # matrixB = pd.DataFrame(matrixBFile).to_numpy()
        matrixA = np.loadtxt(pathA, dtype=type, delimiter=" ")
        # print(matrixA)
        matrixB = np.loadtxt(pathB, dtype=type, delimiter=" ")
        # print(matrixB)
        # matrixC = matrixA*matrixB               # wrong
        matrixC = np.matmul(matrixA, matrixB)  # correct
        np.savetxt(pathC, matrixC, fmt='%f', delimiter=" ")
        # print(matrixC)
