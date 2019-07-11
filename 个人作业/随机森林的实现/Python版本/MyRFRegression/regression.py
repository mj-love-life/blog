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
    # pro2的数据
    train = pd.concat([pd.read_csv('./input/train{}.csv'.format(i), header=None) for i in range(1, 2)], ignore_index=True)
    print("读取训练数据完成")
    label = pd.concat([pd.read_csv('./input/label{}.csv'.format(i), header=None) for i in range(1, 2)], ignore_index=True)
    print("读取label数据完成")
    test = pd.concat([pd.read_csv('./input/test{}.csv'.format(i), header=None) for i in range(1, 2)], ignore_index=True)
    print("读取测试数据完成")
    label.columns = ['label']
    train = pd.merge(train, label, left_index=True, right_index=True)
    features = train.columns[:-1]
    del label
    print("合并训练数据完成")
    folds = KFold(n_splits=5, shuffle=True, random_state=4590)
    predictions = np.zeros(len(test))

    for fold_, (trn_idx, val_idx) in enumerate(folds.split(train[features], train['label'])):
        print("fold {}".format(fold_))
        # n_jobs是线程数
        regr = MyRFRegressor(max_depth=25, n_estimators=250, n_jobs=10)
        t1 = time.time()
        regr.fit(train[features].iloc[trn_idx], train['label'].iloc[trn_idx])
        print('fit time is {}'.format(time.time() - t1))
        print(regr.score(train[features].iloc[val_idx], train['label'].iloc[val_idx]))
        predictions += regr.prediction(test)
    predictions = np.round(predictions / 5, 4)
    predictions = pd.DataFrame({'id': range(1, len(predictions) + 1), 'predicted': predictions})
    predictions.to_csv("lgb_sub.csv", index=False)

    # 用于对比时的样本测例
    # train = pd.read_csv('./input/train1.csv', header=None).head(3000)
    # target = pd.read_csv('./input/label1.csv', header=None).head(3000)

    # folds = KFold(n_splits=5, shuffle=True, random_state=4590)
    # X_train, X_test, y_train, y_test = train_test_split(train, target, test_size = 0.3, random_state = 0)

    # 并行版本的准确率以及时间消耗
    # prediction = np.zeros(X_test.shape[0])
    # start_time = time.time()
    # for fold_, (trn_idx, val_idx) in enumerate(folds.split(X_train, y_train)):
    #     print("fold {}".format(fold_))
    #     regr = MyRFRegressor(n_estimators=100, max_depth=5, n_jobs=4)
    #     t1 = time.time()
    #     regr.fit(X_train.iloc[trn_idx], y_train.iloc[trn_idx])
    #     t2 = time.time()
    #     print('fit time is {}'.format(t2 - t1))
    #     print('accuracy of fold {} is {}'.format(fold_, regr.score(X_train.iloc[val_idx], y_train.iloc[val_idx])))
    #     print('score time is {}'.format(time.time() - t2))
    #     prediction += regr.prediction(X_test)
    # print("r2_score is {}".format(r2_score(y_test, prediction/5)))
    # end_time = time.time()
    # print("My Paralle tree's time is {}".format(end_time - start_time))

    # print('-' * 100)

    # 非并行版本的准确率与时间消耗
    # prediction = np.zeros(X_test.shape[0])
    # start_time = time.time()
    # for fold_, (trn_idx, val_idx) in enumerate(folds.split(X_train, y_train)):
    #     print("fold {}".format(fold_))
    #     regr = MyRFRegressor(n_estimators=100, max_depth=5, n_jobs=1)
    #     t1 = time.time()
    #     regr.fit(X_train.iloc[trn_idx], y_train.iloc[trn_idx])
    #     t2 = time.time()
    #     print('fit time is {}'.format(t2 - t1))
    #     print('accuracy of fold {} is {}'.format(fold_, regr.score(X_train.iloc[val_idx], y_train.iloc[val_idx])))
    #     print('score time is {}'.format(time.time() - t2))
    #     prediction += regr.prediction(X_test)
    # print("r2_score is {}".format(r2_score(y_test, prediction/5)))
    # end_time = time.time()
    # print("My Not Paralle tree's time is {}".format(end_time - start_time))

    # sklearn的随机森林的准确率与时间消耗
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