import numpy as np
import numpy as nd

def pool2d(X: np.ndarray, pool_size: int, fun = max) -> np.ndarray:
    p_h, p_w = pool_size
    Y = nd.zeros((X.shape[0]-p_h+1, X.shape[1]-p_w+1))
    for i in range(Y.shape[0]):
        for j in range(Y.shape[1]):
            # Y[i, j] = fun(X[i:i+p_h, j:j+p_w])
            Y[i, j] = X[i:i+p_h, j:j+p_w].max()
    return Y
X = nd.arange(9).reshape(3,3)
print(pool2d(X, (2, 2)))

def pool2d_multi_in(X: np.ndarray, pool_size: int):
    return nd.array([*[pool2d(x, pool_size) for x in X]])

X = nd.arange(16).reshape((4,4))
X = nd.array([X, X+1])
print(X)
print(pool2d_multi_in(X, (2, 2)))