# testing filereader
import numpy as np




def populate(rows,cols,coef=10):
	#return np.random.randint(10,size=(rows,cols))
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


def iter_covariance(row,col,elems):
	data = populate(row,col)
	cov = covariance(data)
	elems.append(data)
	elems.append(cov)


def test_covariance():
	covars = []
	iter_covariance(9,5,covars)
	iter_covariance(9,3,covars)
	write('covariance.txt',covars)


def driver():
	test_covariance()

	

if __name__ == '__main__':
	driver()













'''

import numpy as np




def test_covariance():
    
    sample_a = np.array([
        [4.0, 2.0, 0.6],
        [4.2, 2.1, 0.59],
        [3.9, 2.0, 0.58],
        [4.3, 2.1, 0.62],
        [4.1, 2.2, 0.63]
    ]).T
    cov = np.cov(sample_a)
    print(cov)
    
    
    #A = np.array([[5, 0, 3, 7], [1, -5, 7, 3], [4, 9, 8, 10]]).T
    A = np.array([3, 6, 4])
    B = np.array([7,12, -9])
    C = np.array([A,B])

    D = np.array([[2, 0, -9],[3, 4, 1]])
    E = np.array([[5, 2, 6],[-4, 4, 9]])

    F = np.array([2, 0, -9, 3, 4, 1])
    G = np.array([5, 2, 6, -4, 4, 9])

    H = np.array([[2,3],[0,4],[-9,1]])
    I = np.array([[5,-4],[2,4],[6,9]])

    J = np.array([[2,3],[0,4],[-9,1],[5,-4],[2,4],[6,9]])

    K = np.array([2, 0, -9, 3, 4, 1, 4, 3, -5])
    L = np.array([5, 2, 6, -4, 4, 9, -8, 4, 1])

    M = np.array([ [2, 0, -9], [3, 4, 1], [4, 3, -5] ])
    N = np.array([ [5, 2, 6], [-4, 4, 9], [-8, 4, 1] ])

    O = np.array([ [2, 0, -9], [3, 4, 1],[4, 3, -5], [5, 2, 6], [-4, 4, 9], [-8, 4, 1] ])
    

    cov_a = np.cov(A)
    cov_b = np.cov(B)
    cov_ab = np.cov(A,B)
    cov_c = np.cov(C)
    cov_de = np.cov(D,E)
    cov_fg = np.cov(F,G)
    cov_hi = np.cov(H,I)
    cov_h = np.cov(H)
    cov_j = np.cov(J)
    cov_lk = np.cov(K,L)
    cov_mn = np.cov(M,N)
    cov_m = np.cov(M)
    cov_n = np.cov(N)
    cov_o = np.cov(O)

    print(M)
    print(cov_m)

    print(N)
    print(cov_n)
    
    


def driver():
    test_covariance()




if __name__ == '__main__':
    driver()


'''
