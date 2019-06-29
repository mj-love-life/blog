#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <omp.h>
#include "utils.h"
#include "CART.h"
#include <math.h>
#include <float.h>

using namespace std;

class RandomForest {

public:

    // 析构函数
    RandomForest(){

    }
    // 有参数的构造函数
    RandomForest(int n_estimators, int max_depth, int features, string features_methods, int sample_threshold){
        // 决策树的数目
        this->n_estimators = n_estimators;
        // 树的最大深度
        this->max_depth = max_depth;
        // 特征数量
        this->features_num = features;
        // 获取每次抽样的特征数量
        if (features_methods == "sqrt"){
            this->use_features = int(sqrt(features));
        } else {
            this->use_features = int(log(features));
        }
        this->use_features = features;
        // 获取每次分裂时的最小节点样本数量
        this->sample_threshold = sample_threshold;
    }

    // 训练
    void train(const vector<Sample> &train_samples);
    // 预测
    vector<float> predict(const vector< vector<float> > test);
    // 评分
    float score(const vector<float> &y_pred, vector<float> y_true);


private:
    // 决策树的数目
    int n_estimators;
    // 决策树的最大深度
    int max_depth;
    // 总的特征数目
    int features_num;
    // 采样的特征数目
    int use_features;
    // 节点分裂结点
    int sample_threshold;
    // 样本的大小
    int sample_size;

    // 训练样本
    vector<Sample> train_samples;
    // 训练样本索引（用于随机抽取样本）
	vector<int> sample_indices;
	// 特征索引（用于随机抽取特征）
	vector<int> feature_indices;
    // CART决策树集合
    vector<CART> estimators;


	// 随机抽取 random_sample_num 个样本
	vector<Sample*> bagging(void);
	// 随机抽取 random_feature_num 个特征（0~200）
	vector<int> random_select_features(void);
	// 训练一颗 CART
	CART generate_estimator(const int cart_no, vector<Sample*> &samples, vector<int> &features);
	// 递归地分裂节点
	void split_node_recursively(const int cart_no, vector<Sample*> &samples, vector<int> &features, Node *&node, const int depth);
	// 找到一组特征中的最佳分割点
	tuple<float, float> find_split(vector<Sample*> &samples, const int feature);
	// 将样本基于某个特征进行排序

};

