# -*- coding: utf-8 -*-


from __future__ import division
import numpy as np
from scipy.linalg import cholesky


class MerweScaledSigmaPoints(object):


    def __init__(self, n, alpha, beta, kappa):
        self.n = n
        self.alpha = alpha
        self.beta = beta
        self.kappa = kappa
        self.sqrt = cholesky
        self.subtract= np.subtract
        

    def num_sigmas(self):
        return 2*self.n + 1


    def sigma_points(self, x, P):
        assert self.n == np.size(x), "expected size {}, but size is {}".format(
            self.n, np.size(x))

        n = self.n

        if np.isscalar(x):
            x = np.asarray([x])
        if  np.isscalar(P):
            P = np.eye(n)*P
        else:
            P = np.asarray(P)

        lambda_ = self.alpha**2 * (n + self.kappa) - n
        U = self.sqrt((lambda_ + n)*P)

        sigmas = np.zeros((2*n+1, n))
        sigmas[0] = x
        for k in range(n):
            sigmas[k+1]   = self.subtract(x, -U[k])
            sigmas[n+k+1] = self.subtract(x, U[k])

        return sigmas


    def weights(self):
        n = self.n
        lambda_ = self.alpha**2 * (n +self.kappa) - n

        c = .5 / (n + lambda_)
        Wc = np.full(2*n + 1, c)
        Wm = np.full(2*n + 1, c)
        Wc[0] = lambda_ / (n + lambda_) + (1 - self.alpha**2 + self.beta)
        Wm[0] = lambda_ / (n + lambda_)

        return Wm, Wc

