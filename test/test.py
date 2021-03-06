# testing filereader
import numpy as np
import scipy
import scipy.linalg




def populate(rows,cols,coef=10):
	return np.random.random(size=(rows,cols)) * coef

	
def stringify(elem):
	flat = elem.flatten()
	lst = [str(x) for x in flat.tolist()]
	s = ','.join(lst)
	return s
	

def prefix(mtx):
	r,c = mtx.shape[0],mtx.shape[1]
	return 'm/'+str(r)+'/'+str(c)+'/,'


def write(path,elems):
	with open(path,'wt') as f:
		for i in range(0,len(elems),2):
			Q = elems[i]
			A = elems[i+1]
			q = prefix(Q)+stringify(Q)+',\n'
			a = prefix(A)+stringify(A)+',\n'
			f.write(q)
			f.write(a)
			

def covariance(data):
	return np.cov(data,rowvar=False)


def cholesky(data):
	cov = covariance(data)
	return np.linalg.cholesky(cov)


def LUP(data):
	lup = scipy.linalg.lu(data)
	return np.array(lup)


def inverse(data):
	return np.linalg.inv(data)


def execute(row,col,calc,elems):
	data = populate(row,col)
	res = calc(data)
	elems.append(data)
	elems.append(res)
	return data, res


def test_covariance():
	covars = []
	execute(9,5,covariance,covars)
	execute(9,3,covariance,covars)
	write('data/covariance.txt',covars)


def test_cholesky():
	chols = []
	execute(5,5,cholesky,chols)
	execute(7,7,cholesky,chols)
	write('data/cholesky.txt',chols)


def test_LUP():
	decomp = []
	execute(5,5,LUP,decomp)
	execute(7,7,LUP,decomp)
	write('data/lup.txt',decomp)


def test_inverse():
	inv = []
	execute(5,5,inverse,inv)
	execute(7,7,inverse,inv)
	write('data/inverse.txt',inv)


def driver():
	#test_covariance()
	#test_cholesky()
	#test_LUP()
	test_inverse()
	

if __name__ == '__main__':
	driver()









def multiplyABAt(a,b):
	A = np.array(a)
	B = np.array(b)
	AB = np.dot(A,B)
	ABAt = np.dot(AB,A.T)
	print(A)
	print(B)
	print(ABAt)
	

def multiply_test():
	
	mtxA = [[6,5,7,3],
			[7,3,9,8],
			[2,5,1,4]]
	mtxB = [[1,7,4,9],
			[6,5,1,6],
			[7,8,4,1],
			[1,2,7,4]]
	multiplyABAt(mtxA,mtxB)
	
	mtxC = [[6, 5, 7],
			[3, 7, 3],
			[9, 8, 2],
			[5, 1, 4],
			[7, 2, 3]]
	mtxD = [[1, 7, 4],
			[9, 6, 5],
			[1, 6, 7]]
	multiplyABAt(mtxC,mtxD)






# end of file

