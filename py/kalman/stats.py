# -*- coding: utf-8 -*-


from __future__ import (absolute_import, division, print_function,
                        unicode_literals)


import numpy as np
from scipy.stats import norm, multivariate_normal


_support_singular = True
try:
    multivariate_normal.logpdf(1, 1, 1, allow_singular=True)
except:
    _support_singular = False


def logpdf(x, mean=None, cov=1, allow_singular=True):
    if mean is not None:
        flat_mean = np.asarray(mean).flatten()
    else:
        flat_mean = None
    flat_x = np.asarray(x).flatten()
    if _support_singular:
        return multivariate_normal.logpdf(flat_x, flat_mean, cov, allow_singular)
    else:
        return multivariate_normal.logpdf(flat_x, flat_mean, cov)

