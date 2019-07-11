#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <windows.h>

using namespace std;

typedef struct {
    vector<float> fea;
    float label;
} Sample;

string now(void);

void log(const char * log_msg);

int rand(const int begin, const int end);

void shuffle(vector<int> &cards);

Sample deal_train_line(const string &line, int feature_num);

vector<Sample> read_train(const char *path, int feature_num);

vector<float> deal_test_line(const string& line, int feature_num);

vector< vector<float> > read_test(const char *path, int feature_num);

void write_predict(const vector<float> &test_y, const char *path);

vector<float> read_label(const char *path);

float calculate_variance(const vector<Sample*> &samples);
