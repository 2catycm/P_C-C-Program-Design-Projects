import numpy as np
import math

if __name__ == '__main__':
    # 参数
    theta = 1  # 每一个数认定相等的误差。
    for size in [32, 64, 128, 256, 512, 1024, 2048]:
        errorCount = 0  # 有多少个点是错误的。# 要清零！
        print("testing size %d"%size)
        # type = np.typeDict['f']
        type = np.typeDict['d']
        # 读取
        path_expected = "../data/mat-C-%d.txt" % size
        path_actually = "../data/mat-C-%d_float.txt" % size
        matrixA = np.loadtxt(path_expected, dtype=type, delimiter=" ")
        matrixB = np.loadtxt(path_actually, dtype=type, delimiter=" ")
        # 比较
        for i in range(size):
            for j in range(size):
                difference = math.fabs(matrixA[i][j] - matrixB[i][j])
                if difference > theta:
                    # print("Expected: {0:f}, actually {1:f}, at\n ({2:d}, {3:d})".format(matrixA[i][j], matrixB[i][j], i, j))
                    errorCount += 1
        print("Your result has %d elements that is different." % errorCount)
        print("Your computation accuracy is %f %%" % ((1 - (errorCount / (size ** 2))) * 100))

        a = np.linalg.norm(matrixA)
        b = np.linalg.norm(matrixB)
        print("The norm values are expected {0:f} and actually {1:f}, and the difference is {2:f}".format(a, b, a - b))
        print("Your computation accuracy is %f %%" % ((1 - (a - b) / a) * 100))
