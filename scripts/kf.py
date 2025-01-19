#!/usr/bin/python3

"""
kf.py: Kalman filter model
"""

__author__ = "Daniel Kopiecki"
__email__ = "daniel.kopiecki@protonmail.com"

import numpy as np
from numpy.linalg import inv


class kf:
    def __init__(self, f, h, q, r, b):
        self.f = f
        self.h = h
        self.q = q
        self.r = r
        self.b = b

        self.x = np.zeros((f.shape[0], 1), np.float32)
        self.p = np.eye(self.x.shape[0])

    def predict(self, u):
        self.x = self.f @ self.x
        self.p = self.f @ self.p @ self.f.T + self.q

        return self.x

    def update(self, z):
        k = self.p @ self.h.T @ inv(self.h @ self.p @ self.h.T + self.r)

        self.x += k @ (z - self.h @ self.x)
        self.p = self.p - k @ self.h @ self.p

        return self.x
