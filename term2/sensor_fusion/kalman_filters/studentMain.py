from math import *

# def f(mu, sigma2, x):
#     return 1/sqrt(2.0*pi*sigma2) * exp(-0.5*(x-mu)**2 / sigma2)

# # print(f(10.0, 4.0, 10.0))

# """write a program to update your mean and variance"""

# def update(mean1, var1, mean2, var2):
#     new_mean = (mean1*var2 + mean2*var1) / (var1 + var2)
#     new_var = (var1 * var2) / (var1 + var2)
#     return [new_mean, new_var]

# # print(update(10.0, 8.0, 13.0, 2.0))

# """ write a program that will predict your new mean and variance
# given the mean and variance of prior step"""

# def predict(mean1, var1, mean2, var2):
#     new_mean = mean1 + mean2
#     new_var = var1 + var2
#     return [new_mean, new_var]

# # print(predict(10.0, 4.0, 12.0, 4.0))

# """ write a program that will iteratively update and predict based on
# the location measurements and inferred motion"""

# measurements = [5.0, 6.0, 7.0, 9.0, 10.0]
# motion = [1.0, 1.0, 2.0, 1.0, 1.0]
# measurement_sig = 4.0
# motion_sig = 2.0
# mu = 0.0
# sig = 10000.0

# for i in range(len(measurements)):
#     [mu, sig] = update(mu, sig, measurements[i], measurement_sig)
#     [mu, sig] = predict(mu, sig, motion[i], motion_sig)

# # print([mu, sig])

import numpy as np

# Implement the filter function below
def kalman_filter(x, P):
    for i in range(len(measurements)):
        #measurement step
        Z = np.array(measurements[i])
        y = Z - np.dot(H, x)
        S = np.dot(np.dot(H, P), H.transpose()) + R
        K = np.dot(np.dot(P, H.transpose()), np.linalg.inv(S))
        x = x + np.dot(K, y)
        P = np.dot(I - np.dot(K, H), P)

        # predict step
        x = np.dot(F, x) + u
        P = np.dot(np.dot(F, P), F.transpose())
    return x, P

measurements = [1, 2, 3]

x = np.array([[0.], [0.]]) # initial state (location and velocity)
P = np.array([[1000., 0.], [0., 1000.]]) # initial uncertainty
u = np.array([[0.], [0.]]) # external motion
F = np.array([[1., 1.], [0, 1.]]) # next state function
H = np.array([[1., 0.]]) # measurement function
R = np.array([[1.]]) # measurement uncertainty
I = np.array([[1., 0.], [0., 1.]]) # identity matrix

[x_prime, P_prime] = kalman_filter(x, P)

print(x_prime, P_prime)