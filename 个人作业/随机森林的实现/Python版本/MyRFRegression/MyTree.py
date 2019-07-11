import numpy as np
import pandas as pd
import math
from sklearn.tree import DecisionTreeRegressor
from sklearn.metrics import r2_score
import time

class MyTree:
    def __init__(self, max_depth=3, criterion='mse'):
        self.max_depth = max_depth
        self.criterion = criterion
        self.tree = DecisionTreeRegressor(max_depth=max_depth, criterion=criterion)

    def fit(self, data, label):
        '''
            1-随机选择样本
            2-随机选择特征
            3-构建决策树
            4-随机森林投票
        '''
        m, n = data.shape
        row_sample = np.random.choice(m, m)
        '''
        if n < 50:
            self.feature_sample = 
        else:
            self.feature_sample = 
        '''
        self.feature_sample = np.random.choice(n , int(math.sqrt(n)), replace=False)
        self.data = data[row_sample, :]
        print('-' * 100)
        print('测试样例是:', row_sample)
        self.label = label[row_sample]
        # 随机特征的选择，这里的特征选择是无放回的选择
        self.data = self.data[:, self.feature_sample]
        # t1 = time.time()
        self.tree.fit(self.data, self.label.ravel())
        # print('开数据的时间消耗为{}'.format(time.time() - t1))
        return self
    
    def predict(self, data):
        return self.tree.predict(data[:, self.feature_sample])
