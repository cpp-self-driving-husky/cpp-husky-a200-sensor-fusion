# unscented testing


from kalman.sigma_points import MerweScaledSigmaPoints
from kalman.UKF import UnscentedKalmanFilter as UKF
import numpy as np
from numpy.random import randn


def f_cv(x, dt):
    """ state transition function for a 
    constant velocity aircraft"""
    return np.array([x[0]+0.7, x[1]+0.5, 0.7, 0.5])
    

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

	zs = [np.array([xi+i*xd,yi+i*yd]) for i in range(1,11)]
	
	print(ukf.x)
	print(ukf.P)

	return

	uxs = []
	for z in zs:
		ukf.predict()
		ukf.update(z)

		print(ukf.x)
		print(ukf.P)
		print()

	
if __name__ == '__main__':
	run()

