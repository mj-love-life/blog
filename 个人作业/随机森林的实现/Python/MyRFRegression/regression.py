from MyRFRegressor import MyRFRegressor
import numpy as np
import pandas as pd
import warnings
from sklearn.datasets import load_diabetes
from sklearn.model_selection import KFold, train_test_split
warnings.filterwarnings('ignore')
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import r2_score
import time
from sklearn.utils import shuffle


def main():
    # data = load_diabetes()
    # train = pd.DataFrame(data = data.data)
    # target = pd.DataFrame(data = data.target)
    train = pd.read_csv('./input/train1.csv', header=None).head(3000)
    target = pd.read_csv('./input/label1.csv', header=None).head(3000)

    folds = KFold(n_splits=5, shuffle=True, random_state=4590)
    X_train, X_test, y_train, y_test = train_test_split(train, target, test_size = 0.3, random_state = 0)
    

    prediction = np.zeros(X_test.shape[0])
    start_time = time.time()
    for fold_, (trn_idx, val_idx) in enumerate(folds.split(X_train, y_train)):
        print("fold {}".format(fold_))
        regr = MyRFRegressor(n_estimators=100, max_depth=5, n_jobs=4)
        t1 = time.time()
        regr.fit(X_train.iloc[trn_idx], y_train.iloc[trn_idx])
        t2 = time.time()
        print('fit time is {}'.format(t2 - t1))
        print('accuracy of fold {} is {}'.format(fold_, regr.score(X_train.iloc[val_idx], y_train.iloc[val_idx])))
        print('score time is {}'.format(time.time() - t2))
        prediction += regr.prediction(X_test)
    print("r2_score is {}".format(r2_score(y_test, prediction/5)))
    end_time = time.time()
    print("My Paralle tree's time is {}".format(end_time - start_time))

    print('-' * 100)


    prediction = np.zeros(X_test.shape[0])
    start_time = time.time()
    for fold_, (trn_idx, val_idx) in enumerate(folds.split(X_train, y_train)):
        print("fold {}".format(fold_))
        regr = MyRFRegressor(n_estimators=100, max_depth=5, n_jobs=1)
        t1 = time.time()
        regr.fit(X_train.iloc[trn_idx], y_train.iloc[trn_idx])
        t2 = time.time()
        print('fit time is {}'.format(t2 - t1))
        print('accuracy of fold {} is {}'.format(fold_, regr.score(X_train.iloc[val_idx], y_train.iloc[val_idx])))
        print('score time is {}'.format(time.time() - t2))
        prediction += regr.prediction(X_test)
    print("r2_score is {}".format(r2_score(y_test, prediction/5)))
    end_time = time.time()
    print("My Not Paralle tree's time is {}".format(end_time - start_time))

    # prediction = np.zeros(X_test.shape[0])
    # start_time = time.time()
    # for fold_, (trn_idx, val_idx) in enumerate(folds.split(X_train, y_train)):
    #     print("fold {}".format(fold_))
    #     regr = RandomForestRegressor(n_estimators=100, max_depth=5, n_jobs=2, max_features='sqrt')
    #     t1 = time.time()
    #     regr.fit(X_train.iloc[trn_idx], y_train.iloc[trn_idx])
    #     t2 = time.time()
    #     print('fit time is {}'.format(t2 - t1))
    #     print('accuracy of fold {} is {}'.format(fold_, regr.score(X_train.iloc[val_idx], y_train.iloc[val_idx])))
    #     print('score time is {}'.format(time.time() - t2))
    #     prediction += regr.predict(X_test)
    # print("r2_score is {}".format(r2_score(y_test, prediction/5)))
    # end_time = time.time()
    # print("RF's time is {}".format(end_time - start_time))


if __name__ == "__main__":
    main()


'''
X_train['target'] = y_train
X_train_unique = shuffle(X_train)
y_train_unique = X_train_unique['target']
X_train_unique.drop(columns='target', inplace=True)
index = X_train_unique.shape[0] // 5
prediction = np.zeros(X_test.shape[0])
prediction2 = np.zeros(X_test.shape[0])
for i in  range(5):
    fold_ = i
    print("fold {}".format(fold_))
    regr = MyRFRegressor(n_estimators=100, max_depth=5, n_jobs=2)
    temp_train = pd.DataFrame(data = X_train_unique.values[index * i: index * (i + 1), :])
    temp_target = pd.DataFrame(data = y_train_unique.values[index * i: index * (i + 1)])
    
    t1 = time.time()
    regr.fit(temp_train, temp_target)
    print('accuracy of fold {} is {}'.format(fold_, regr.score(temp_train, temp_target)))
    print('fit and score time is {}'.format(time.time() - t1))
    prediction += regr.prediction(X_test)
    regr2 = RandomForestRegressor(n_estimators=100, max_depth=5, n_jobs=2, max_features='sqrt')
    
    t1 = time.time()
    regr2.fit(temp_train, temp_target)
    print('accuracy of fold {} is {}'.format(fold_, regr2.score(temp_train, temp_target)))
    print('fit and score time is {}'.format(time.time() - t1))
    prediction2 += regr2.predict(X_test)
    print('-' * 100 )
print("r2_score is {}".format(r2_score(y_test, prediction/5)))
print("r2_score is {}".format(r2_score(y_test, prediction2/5)))
pass
'''