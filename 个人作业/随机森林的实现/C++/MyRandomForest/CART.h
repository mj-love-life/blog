#pragma once

#include<iostream>
#include<vector>
#include "utils.h"

using namespace std;

struct Node {
    // 分裂的特征
	int feature;
	// 分裂特征所对应的值
	float split_point;
	// 当前节点所代表的回归值的大小
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
    // 根节点
	Node *root;

	// 构造函数
	CART() {
		root = NULL;
	}

	// 根据特征返回对应的节点的值
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
