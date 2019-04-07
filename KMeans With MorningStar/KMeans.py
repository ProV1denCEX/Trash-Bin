import random
import numpy as np
import pandas as pd


class Cluster(object):
    def __init__(self, num, dim):
        self.num = num
        self.dim = dim
        self.member = {}
        self.centroid = np.array([random.random() for _ in range(0, self.dim)])


class KMeans(object):
    def __init__(self, k=1, tolerance=0.1, max_iter=100):
        self._k = k
        self._tolerance = tolerance
        self._max_iter = max_iter
        self.cluster = []

    @property
    def k(self):
        return self._k

    @property
    def tolerance(self):
        return self._tolerance

    @property
    def max_iter(self):
        return self._max_iter

    def fit(self, data2fit):
        cluster = [Cluster(x, data2fit.shape[1]) for x in range(0, self._k)]

        for _ in range(self._max_iter):
            for i in range(0, data2fit.__len__()):
                distance = [
                    np.linalg.norm(
                        data2fit.iloc[i] -
                        x.centroid) for x in cluster]
                cluster[distance.index(min(distance))
                        ].member[i] = data2fit.iloc[[i]]

            for i in cluster:
                temp = pd.DataFrame(columns=data2fit.columns)
                for j in i.member.values():
                    temp = pd.concat([temp, j])

                new_centroid = np.array(temp.mean())

                if np.sum(np.abs(new_centroid - i.centroid)) / i.dim <= self._tolerance:
                    i.centroid = new_centroid
                    self.cluster = cluster
                    break

                else:
                    i.centroid = new_centroid

        self.cluster = cluster
