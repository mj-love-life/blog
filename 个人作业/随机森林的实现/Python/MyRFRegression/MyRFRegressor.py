import numpy as np
import pandas as pd
from MyTree import MyTree
from multiprocessing.pool import Pool
from sklearn.metrics import r2_score
import pp
import time
from joblib import Parallel, delayed


def _parallel_build_trees(tree, data, label):
    tree.fit(data, label)
    return tree



class MyRFRegressor:
    def __init__(self, n_estimators = 100, max_depth = 5, n_jobs = 2):
        self._n_estimators = n_estimators
        self._max_depth = max_depth
        self._n_jobs = n_jobs
        self._trees = []

    def _decision_tree_fit(self, tree):
        return tree.fit(self.data, self.label)

    def fit(self, data, label):
        self.data = np.asarray(data)
        self.label = np.asarray(label)
        for _ in range(self._n_estimators):
            self._trees.append(MyTree(self._max_depth, criterion='mse'))
        if self._n_jobs == 0:
            for tree in self._trees:
                self._decision_tree_fit(tree)
        else:
            nthread = self._n_jobs
            if self._n_jobs == -1 or self._n_jobs > 4:
                nthread = 4
            # joblib
            # t1 = time.time()
            self._trees=Parallel(n_jobs=nthread, backend='threading')( # do not use backend="threading"
                delayed(_parallel_build_trees)(tree, self.data, self.label) for tree in self._trees)
            # print('parallel time is {}'.format(time.time() - t1))
            
            
            '''
            ------ 并发
                pool = Pool(processes=nthread)
                self._trees = list(pool.map(self._decision_tree_fit, self._trees))

            ------ pp库，串行
                self.job_server = pp.Server()
                t1 = time.time()
                res = []
                self.job_server.set_ncpus(nthread)
                for tree in self._trees:
                    # res.append(self.job_server.submit(func=_parallel_build_trees, args=(tree, self.data, self.label), depfuncs=(), modules=('MyTree', )))
                    temp = self.job_server.submit(func=_parallel_build_trees, args=(tree, self.data, self.label), depfuncs=(), modules=('MyTree', ))
                    if temp.finished:
                        print(temp())
                print('time is {}'.format(time.time() - t1))
                self.job_server.wait()
                for i in range(len(res)):
                    self._trees[i] = res[i]()
                # self._trees = [temp() for temp in res]
                self.job_server.print_stats()
                self.job_server.destroy()
            '''


    def prediction(self, data):
        data = np.asarray(data)
        answers = np.zeros(data.shape[0])
        for tree in self._trees:
            answers += tree.predict(data)
        answers = answers / self._n_estimators
        return answers

    def score(self, data, y_true):
        return r2_score(y_true, self.prediction(data))
