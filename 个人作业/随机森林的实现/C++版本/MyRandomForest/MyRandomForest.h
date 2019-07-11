#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <omp.h>
#include "utils.h"
#include <math.h>
#include <float.h>

using namespace std;

struct Node {
    // ���ѵ�����
	int feature;
	// ������������Ӧ��ֵ
	float split_point;
	// ��ǰ�ڵ�������Ļع�ֵ�Ĵ�С
	float value;
	Node *left;
	Node *right;

	Node() {
		feature = -1;
		value = false;
		split_point = 0.0;
		left = NULL;
		right = NULL;
	}

	bool is_leaf() const {
		return left == NULL && right == NULL;
	}
};

struct CART {
    // ���ڵ�
	Node *root;

	// ���캯��
	CART() {
		root = NULL;
	}

	// �����������ض�Ӧ�Ľڵ��ֵ
	float get_val(const vector<float> &x) const {
		Node *node = root;
		while (!node->is_leaf()) {  // while not a leaf node
			if (node->left == NULL) {
                node = node->right;
			}
			else if (node->right == NULL) {
                node = node->left;
			}
			else {
				node = x[node->feature] < node->split_point ? node->left : node->right;
			}
		}
		return node->value;
	}
};




class RandomForest {

public:

    // ��������
    RandomForest(){

    }
    // �в����Ĺ��캯��
    RandomForest(int n_estimators, int max_depth, int features, string features_methods, int sample_threshold){
        // ����������Ŀ
        this->n_estimators = n_estimators;
        // ����������
        this->max_depth = max_depth;
        // ��������
        this->features_num = features;
        // ��ȡÿ�γ�������������
        if (features_methods == "sqrt"){
            this->use_features = int(sqrt(features));
        } else {
            this->use_features = int(log(features));
        }
        this->use_features = features;
        // ��ȡÿ�η���ʱ����С�ڵ���������
        this->sample_threshold = sample_threshold;
    }

    // ѵ��
    void train(const vector<Sample> &train_samples);
    // Ԥ��
    vector<float> predict(const vector< vector<float> > test);
    // ����
    float score(const vector<float> &y_pred, vector<float> y_true);


private:
    // ����������Ŀ
    int n_estimators;
    // ��������������
    int max_depth;
    // �ܵ�������Ŀ
    int features_num;
    // ������������Ŀ
    int use_features;
    // �ڵ���ѽ��
    int sample_threshold;
    // �����Ĵ�С
    int sample_size;

    // ѵ������
    vector<Sample> train_samples;
    // ѵ���������������������ȡ������
	vector<int> sample_indices;
	// �������������������ȡ������
	vector<int> feature_indices;
    // CART����������
    vector<CART> estimators;


	// �����ȡ random_sample_num ������
	vector<Sample*> bagging(void);
	// �����ȡ random_feature_num ��������0~200��
	vector<int> random_select_features(void);
	// ѵ��һ�� CART
	CART generate_estimator(const int cart_no, vector<Sample*> &samples, vector<int> &features);
	// �ݹ�ط��ѽڵ�
	void split_node_recursively(const int cart_no, vector<Sample*> &samples, vector<int> &features, Node *&node, const int depth);
	// �ҵ�һ�������е���ѷָ��
	tuple<float, float> find_split(vector<Sample*> &samples, const int feature);
	// ����������ĳ��������������

};

