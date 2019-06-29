#include <stdio.h>
#include <vector>
#include "utils.h"
#include "CART.h"
#include "MyRandomForest.h"

#define TRAIN_DATA  "C:\\Users\\Horseplus\\Desktop\\随机森林\\RF\\MyRFRegression\\input\\1.csv"
#define TEST_DATA "C:\\Users\\Horseplus\\Desktop\\随机森林\\RF\\MyRFRegression\\input\\test.csv"
#define LABEL_DATA "C:\\Users\\Horseplus\\Desktop\\随机森林\\RF\\MyRFRegression\\input\\label.csv"

char main_log[256];


int main(void) {
	// read train data
	printf("%s begin to read train data from file: %s", main_log, TRAIN_DATA);
	log(main_log);
	vector<Sample> train_samples = read_train(TRAIN_DATA, 13);
	vector<Sample*> samples;


//	const int total = 3000;
//	for (int i = 0; i < 3000; ++i) {
//		samples.push_back(&train_samples[i]);
//	}
//	printf("%f\n", calculate_variance(samples));



	vector< vector<float> > test_samples = read_test(TEST_DATA, 13);
	vector<float> test_label = read_label(LABEL_DATA);
//    for(auto i: test_label){
//        printf("%f", i);
//    }
    RandomForest rf = RandomForest(4, 10, 13, "sqrt", 10);
    rf.train(train_samples);
//    vector<float> pred = rf.predict(test_samples);
//    cout << "hhhh" << endl;
//    rf.score(pred, test_label);
	return 0;
}
