#pragma once

#include<iostream>
#include<vector>
#include "utils.h"

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
