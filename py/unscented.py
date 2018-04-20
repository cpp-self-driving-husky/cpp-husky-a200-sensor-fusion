# -*- coding: utf-8 -*-


from kalman.sigma_points import MerweScaledSigmaPoints
from kalman.UKF import UnscentedKalmanFilter as UKF
import numpy as np
from numpy.random import randn


def f_cv(x, dt, *args):
	ctrl = args[0]
	return np.array([x[0]+ctrl[0], x[1]+ctrl[1], x[2], x[3]])


def h_cv(x):
	return np.array([x[0], x[1]])


def run():

	N = 4
	dt = 1.0
	xi,yi = 78.6,191.2
	xd,yd = 0.7,0.5

	sigmas = MerweScaledSigmaPoints(N, alpha=.5, beta=2., kappa=1.)

	ukf = UKF(dim_x=N, dim_z=2, fx=f_cv,
			  hx=h_cv, dt=dt, points=sigmas)
	ukf.x = np.array([xi, yi, xd, yd])
	ukf.P = np.array([[1.0, 0.2, 0.2, 0.2],
					  [0.2, 1.0, 0.2, 0.2],
					  [0.2, 0.2, 1.0, 0.2],
					  [0.2, 0.2, 0.2, 1.0]])
	
	ctrl = np.array([xd,yd])
	zs = [np.array([xi+i*xd,yi+i*yd]) for i in range(1,11)]
	
	#print(ukf.x)
	#print(ukf.P)
	
	uxs = []
	for z in zs:
		ukf.predict(fx_args=(ctrl))
		
		print(ukf.sigmas_f)
		return
		
		ukf.update(z)


	
if __name__ == '__main__':
	run()

