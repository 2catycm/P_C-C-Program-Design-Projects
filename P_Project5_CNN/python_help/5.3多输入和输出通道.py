import numpy as nd
import numpy as np
def corr2d(X: nd.ndarray, K):  # input, kernel
    h, w = K.shape
    Y = nd.zeros((X.shape[0] - h + 1, X.shape[1] - w + 1))
    for i in range(Y.shape[0]):
        for j in range(Y.shape[1]):
            Y[i, j] = (X[i:i + h, j:j + w] * K).sum()
    return Y
def corr2d_multi_in(X, K):
    return nd.add(*[corr2d(x, k) for x, k in zip(X, K)])
# def corr2d_multi_in2(X, K):
#     _, h, w = K.shape  # 窗口的大小
#     Y = np.zeros((X.shape[0] - h + 1, X.shape[1] - w + 1))

X = nd.array([[ [0,1, 2], [3,4,5], [6,7,8]],
              [[1,2,3], [4, 5, 6], [7,8,9] ]])
K = nd.array([ [ [0,1], [2, 3]], [[1,2], [3,4]]])
print(corr2d_multi_in(X, K))

def corr2d_multi_in_out(X, K):
    return nd.array([*[corr2d_multi_in(X, k) for k in K]])
K = nd.array([K, K+1, K+2])
print(K.shape)
print(corr2d_multi_in_out(X, K))

