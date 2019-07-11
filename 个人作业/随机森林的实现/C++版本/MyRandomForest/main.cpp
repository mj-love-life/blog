#include <stdio.h>
#include <vector>
#include "utils.h"
#include "MyRandomForest.h"
#define TRAIN_DATA  "./input/train.csv"
#define TEST_DATA "./input/test.csv"
#define LABEL_DATA "./input/label.csv"

int main(void) {
	// read train data
	vector<Sample> train_samples = read_train(TRAIN_DATA, 13);
	vector<vector<float>>  train_fea;
    vector<float>  train_label;
	const int total = train_samples.size();
	for (int i = 0; i < total; ++i) {
		train_fea.push_back(train_samples[i].fea);
		train_label.push_back(train_samples[i].label);
	}
	printf("finish loading train data\n");
	// 初始化随机森林以及进行训练
    RandomForest rf = RandomForest(4, 10, 13, "sqrt", 10);
    rf.train(train_samples);
    printf("finish training data\n");
    printf("predicting .....\n");
    vector<float> pred = rf.predict(train_fea);
    float cal = rf.score(pred, train_label);
    printf("r2_score is %f\n", cal);

    // 测试集的检验
    vector< vector<float> > test_sample = read_test(TEST_DATA, 13);
    vector<float> label_sample = read_label(LABEL_DATA);
    printf("finish loading test data\n");
    pred = rf.predict(test_sample);
    printf("predicting .....\n");
    cal = rf.score(pred, label_sample);
    printf("r2_score is %f\n", cal);
	return 0;
}
