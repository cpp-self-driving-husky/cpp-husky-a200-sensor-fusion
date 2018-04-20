# -*- coding: utf-8 -*-


from __future__ import (absolute_import, division)

import math
import numpy as np
from numpy import eye, zeros, dot, isscalar, outer
from scipy.linalg import inv, cholesky
import sys
from kalman.stats import logpdf


def unscented_transform(sigmas, Wm, Wc, noise_cov=None,
                        mean_fn=None, residual_fn=None):
							
    kmax, n = sigmas.shape
    if mean_fn is None:
        x = np.dot(Wm, sigmas)
    else:
        x = mean_fn(sigmas, Wm)

    if residual_fn is np.subtract or residual_fn is None:
        y = sigmas - x[np.newaxis,:]
        P = np.dot(y.T, np.dot(np.diag(Wc), y))
    else:
        P = np.zeros((n, n))
        for k in range(kmax):
            y = residual_fn(sigmas[k], x)
            P += Wc[k] * np.outer(y, y)

    if noise_cov is not None:
        P += noise_cov

    return (x, P)


class UnscentedKalmanFilter:


    def __init__(self, dim_x, dim_z, dt, hx, fx, points,
                 x_mean_fn=None, z_mean_fn=None,
                 residual_x=None, residual_z=None,
                 compute_log_likelihood=True):

        self.Q = eye(dim_x)
        self.R = eye(dim_z)
        self.x = zeros(dim_x)
        self.P = eye(dim_x)
        self._dim_x = dim_x
        self._dim_z = dim_z
        self.points_fn = points
        self._dt = dt
        self._num_sigmas = points.num_sigmas()
        self.hx = hx
        self.fx = fx
        self.x_mean = x_mean_fn
        self.z_mean = z_mean_fn

        self.compute_log_likelihood = compute_log_likelihood
        self.log_likelihood = math.log(sys.float_info.min)

        self.msqrt = cholesky
        self.Wm, self.Wc = self.points_fn.weights()

        if residual_x is None:
            self.residual_x = np.subtract
        else:
            self.residual_x = residual_x
        if residual_z is None:
            self.residual_z = np.subtract
        else:
            self.residual_z = residual_z

        self.sigmas_f = zeros((self._num_sigmas, self._dim_x))
        self.sigmas_h = zeros((self._num_sigmas, self._dim_z))


    def predict(self, dt=None,  UT=None, fx_args=()):

        if dt is None:
            dt = self._dt

        if not isinstance(fx_args, tuple):
            fx_args = (fx_args,)

        if UT is None:
            UT = unscented_transform

        self.compute_process_sigmas(dt, *fx_args)

        self.x, self.P = UT(self.sigmas_f, self.Wm, self.Wc, self.Q,
                            self.x_mean, self.residual_x)



    def update(self, z, R=None, UT=None, hx_args=()):
        """ Update the UKF with the given measurements. On return,
        self.x and self.P contain the new mean and covariance of the filter.

        Parameters
        ----------

        z : numpy.array of shape (dim_z)
            measurement vector

        R : numpy.array((dim_z, dim_z)), optional
            Measurement noise. If provided, overrides self.R for
            this function call.

        UT : function(sigmas, Wm, Wc, noise_cov), optional
            Optional function to compute the unscented transform for the sigma
            points passed through hx. Typically the default function will
            work - you can use x_mean_fn and z_mean_fn to alter the behavior
            of the unscented transform.

        hx_args : tuple, optional, default (,)
            arguments to be passed into Hx function after the required state
            variable.
        """

        if z is None:
            return

        if not isinstance(hx_args, tuple):
            hx_args = (hx_args,)

        if UT is None:
            UT = unscented_transform

        if R is None:
            R = self.R
        elif isscalar(R):
            R = eye(self._dim_z) * R
            
            
            
        # below for testing
        sigmas_t = self.points_fn.sigma_points(self.x,self.P)
        self.sigmas_f = sigmas_t
        # above for testing



        for i, s in enumerate(self.sigmas_f):
            self.sigmas_h[i] = self.hx(s, *hx_args)
        
        # mean and covariance of prediction passed through unscented transform
        zp, Pz = UT(self.sigmas_h, self.Wm, self.Wc, R, self.z_mean, self.residual_z)

        # compute cross variance of the state and the measurements
        Pxz = self.cross_variance(self.x, zp, self.sigmas_f, self.sigmas_h)
        
        self.K = dot(Pxz, inv(Pz))        # Kalman gain
        self.y = self.residual_z(z, zp)   # residual

        # update Gaussian state estimate (x, P)
        self.x = self.x + dot(self.K, self.y)
        self.P = self.P - dot(self.K, dot(Pz, self.K.T))

        if self.compute_log_likelihood:
            self.log_likelihood = logpdf(x=self.y, cov=Pz)


    def cross_variance(self, x, z, sigmas_f, sigmas_h):
        Pxz = zeros((sigmas_f.shape[1], sigmas_h.shape[1]))
        N = sigmas_f.shape[0]
        for i in range(N):
            dx = self.residual_x(sigmas_f[i], x)
            dz =  self.residual_z(sigmas_h[i], z)
            Pxz += self.Wc[i] * outer(dx, dz)
        return Pxz


    def compute_process_sigmas(self, dt, *fx_args):
        sigmas = self.points_fn.sigma_points(self.x, self.P)
        for i, s in enumerate(sigmas):
            self.sigmas_f[i] = self.fx(s, dt, *fx_args)


    @property
    def likelihood(self):
        lh = math.exp(self.log_likelihood)
        if lh == 0:
             lh = sys.float_info.min
        return lh


