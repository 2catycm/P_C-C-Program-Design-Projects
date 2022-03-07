import numpy as np
import torch.nn as nn


def corr2d2(X: np.ndarray, K: np.ndarray) -> np.ndarray:  # input, kernel
    h, w = K.shape  # 窗口的大小
    Y = np.zeros((X.shape[0] - h + 1, X.shape[1] - w + 1))
    X_ = np.zeros((Y.shape[0] * Y.shape[1], h * w))
    k = 0
    for i in range(Y.shape[0]):
        for j in range(Y.shape[1]):
            X_[k] = X[i:i + h, j:j + w].reshape(1, h * w)
            k += 1
    return X_.dot(K.reshape(h * w, 1)).reshape(Y.shape[0], Y.shape[1])


def corr2d(X: np.ndarray, K):  # input, kernel
    h, w = K.shape
    Y = np.zeros((X.shape[0] - h + 1, X.shape[1] - w + 1))
    for i in range(Y.shape[0]):
        for j in range(Y.shape[1]):
            Y[i, j] = (X[i:i + h, j:j + w] * K).sum()
    return Y


# class Conv2d(nn.Module):
#     def __init__(self, kernel_size, **kwargs):
#         super(Conv2d, self).__init__(**kwargs)
#         self.weight = self.params.get('weight', shape=kernel_size)
#         self.bias = self.params.get('bias', shape=1,)
#     def forward(self, x):
#         return corr2d(x, np.ndarray(self.weight.data())) + self.bias.data()
X = np.ones((6, 8))
X[:, 2:6] = 0
print(X)
K = np.array([[1, -1]])
Y = corr2d(X, K)
print(Y)
Y2 = corr2d2(X, K)
print(Y2)
print(Y2 == Y)
