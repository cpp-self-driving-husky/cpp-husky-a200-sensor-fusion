# -*- coding: utf-8 -*-


from kalman.sigma_points import MerweScaledSigmaPoints
from kalman.UKF import UnscentedKalmanFilter as UKF
import numpy as np
from numpy.random import randn


def f_cv(x, dt, *args):
	delta_t = args[0][0]
	vx_i = x[2]
	vy_i = x[3]
	x_i = vx_i * delta_t
	y_i = vy_i * delta_t
	return np.array([x_i, y_i, vx_i, vy_i])


def h_cv(x):
	return np.array([x[0], x[1]])


def get_measurements():
	with open('../test/data/raw_odom_9-13_filtered.csv', 'rt') as fobj:
		msr = [x.split(',') for x in fobj]
	msr = [(float(x[0]), float(x[1]), float(x[2])) for x in msr]
	return msr


def run():

	N = 4
	M = 2
	dt = 1.0
	# xi,yi = 78.6,191.2
	# xd,yd = 0.7,0.5

	data = get_measurements()
	xi = data[0][0]
	yi = data[0][1]
	xd = 0.0
	yd = 0.0

	sigmas = MerweScaledSigmaPoints(N, alpha=.5, beta=2., kappa=1.)

	ukf = UKF(dim_x=N, dim_z=2, fx=f_cv,
			  hx=h_cv, dt=dt, points=sigmas)
	ukf.x = np.array([xi, yi, xd, yd])
	ukf.P = np.array([[1.0, 0.2, 0.2, 0.2],
					  [0.2, 1.0, 0.2, 0.2],
					  [0.2, 0.2, 1.0, 0.2],
					  [0.2, 0.2, 0.2, 1.0]])

	ctrl = np.array([data[0][2]])
	meas = np.array([xi, yi])
	# zs = [np.array([xi+i*xd,yi+i*yd]) for i in range(1,11)]

	print('init')
	print(ukf.x)
	print(ukf.P)
	print()

	for i, z in enumerate(data):
		ctrl[0] = z[2]
		meas[0] = z[0]
		meas[1] = z[1]

		ukf.predict(fx_args=(ctrl))
		ukf.update(meas)

	print('final')
	print(ukf.x)
	print(ukf.P)
	print()



if __name__ == '__main__':
	run()
