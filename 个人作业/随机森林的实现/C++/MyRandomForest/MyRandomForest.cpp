#include "MyRandomForest.h"

char rf_log[256];
//// ����������Ŀ
//int n_estimators;
//// ��������������
//int max_depth;
//// �ܵ�������Ŀ
//int features;
//// ������������Ŀ
//int use_features;
//// �ڵ���ѽ��
//int sample_threshold;
//// �����Ĵ�С
//int sample_size;
//// ѵ������
//vector<Sample> train_samples;
//// ѵ���������������������ȡ������
//vector<int> sample_indices;
//// �������������������ȡ������
//vector<int> feature_indices;
//// CART����������
//vector<CART> estimators;
// ѵ��
void RandomForest::train(const vector<Sample> &train_samples){

    // ��ʼ��
    this->train_samples = train_samples;
    this->sample_size = train_samples.size();
    this->sample_indices.resize(this->sample_size);
    for (int i = 0; i < this->sample_size; i++){
        this->sample_indices[i] = i;
    }
    this->feature_indices.resize(this->features_num);
    for (int i = 0; i < this->features_num; i++) {
        this->feature_indices[i] = i;
    }
    this->estimators.resize(this->n_estimators);

    omp_set_num_threads(4);
    #pragma omp parallel for
    for (int i = 0; i < this->n_estimators; ++i) {
        vector<Sample*> samples = bagging();
		vector<int> features = random_select_features();
		this->estimators[i] = generate_estimator(i + 1, samples, features);
	}
}

// bagging��������
vector<Sample*> RandomForest::bagging(void){
    vector<Sample*> samples;
    for(int i =0; i < this->sample_size; i++) {
        int temp = rand(0, this->sample_size);
        samples.push_back(&this->train_samples[temp]);
    }
    return samples;
}

// �����ȡ this.use_features; ������
vector<int> RandomForest::random_select_features(void) {
    vector<int> features;
    shuffle(this->feature_indices);
    int temp = rand(0, this->features_num);
    for (int i = 0; i < this->use_features; i++) {
        features.push_back(this->feature_indices[(temp + i) % this->features_num]);
    }
    return features;
}

// ѵ��һ�� CART
CART RandomForest::generate_estimator(const int cart_no, vector<Sample*> &samples, vector<int> &features){
    printf("the thread %d and estimator %d is training\n", omp_get_thread_num(), cart_no);
    CART estimator;
    split_node_recursively(cart_no, samples, features, estimator.root, 0);
    printf("the thread %d and estimator %d finish\n", omp_get_thread_num(), cart_no);
    return estimator;
}

// �ݹ�ط��ѽڵ�
void RandomForest::split_node_recursively(const int cart_no, vector<Sample*> &samples, vector<int> &features, Node *&node, const int depth){
    // ��ֹ����
    if (depth >= max_depth || samples.size() < this->sample_threshold) {
        return ;
    }
    // ���㵱ǰ�ķ����Ա�ȷ���Ƿ���л���
    float cur_var = calculate_variance(samples);
    float best_split_point = 0.0f;
    float min_var = cur_var;
    int min_fea_index = -1;
    for (int i = 0; i < features.size(); i++) {
        int feature = features[i];
        if (feature == -1) {
            continue;
        }
        tuple<float, float> split_info = find_split(samples, feature);
        float split_point = get<0>(split_info);
        float var = get<1>(split_info);
        if (var <min_var){
            min_fea_index = i;
            best_split_point = split_point;
            min_var = var;
        }
    }
    if (min_fea_index == -1) {
        return;
    }

    node = new Node();
    node->feature = features[min_fea_index];
    node->split_point = best_split_point;
    vector<Sample*> l_samples;
    vector<Sample*> r_samples;

    // �ָ�Ϊ�����ֽ��еݹ�
    for (Sample* sample: samples){
        if(sample->fea[features[min_fea_index]] <= best_split_point) {
            l_samples.push_back(sample);
        }
        else{
            r_samples.push_back(sample);
        }
    }
    // �����ٴηָ�
    features[min_fea_index] = -1;
    split_node_recursively(cart_no, l_samples, features, node->left, depth + 1);
    split_node_recursively(cart_no, r_samples, features, node->right, depth + 1);


    if(node ->is_leaf()){
        float sum = 0;
        for (Sample* sample: samples){
            sum += sample->label;
        }
        node->value = (float) sum/samples.size();
    }
}

// �ҵ�һ�������е���ѷָ��
tuple<float, float> RandomForest::find_split(vector<Sample*> &samples, const int feature){
    sort(samples.begin(), samples.end(), [=](const auto &s1, const auto &s2) { return s1->fea[feature] < s2->fea[feature]; });
    float min_variance = FLT_MAX;
	float best_split_point = 0.0f;
	float total_sum = 0;
	int sample_size_temp = samples.size();
	for (int i = 0; i < sample_size_temp; i++) {
        total_sum += samples[i]->label;
	}
	float arr_sum0 = 0;
	float arr_sum1 = 0;
	for (int i = 0 ; i < sample_size_temp - 1; ++i) {
		float split_point = (samples[i]->fea[feature] + samples[i + 1]->fea[feature]) / 2;
		int size0 = i + 1;
		int size1 = sample_size_temp - size0;
		arr_sum0 += samples[i]->label;
		arr_sum1 = total_sum - arr_sum0;
        float sum0 = 0;
        float sum1 = 0;
        float mean0 = (float) arr_sum0 / size0;
        float mean1 = (float) arr_sum1 / size1;
        //printf("mean1 and mean0 here is%f and %f\n", mean1, mean0);
        for (int j = 0; j <= i; j++){
            sum0 += pow(samples[j]->label - mean0, 2);
        }
        for (int j = i + 1; j < sample_size_temp; j++) {
            sum1 += pow(samples[j]->label - mean1, 2);
        }
        float var = sum0 + sum1;
        if (i > this->sample_threshold and sample_size_temp - i > this->sample_threshold){
            if (var < min_variance) {
                min_variance = var;
                best_split_point = split_point;
            }
        }
	}
	return make_tuple(best_split_point, min_variance);
}

// Ԥ��
vector<float> RandomForest::predict(const vector< vector<float> > test){
    vector<float> pred;
    for (const vector<float> &fea : test) {
        float p = 0;
        for (const CART &estimator : this->estimators) {
            p += estimator.get_val(fea);
        }
        pred.push_back(p/this->n_estimators);
	}
	for(float i: pred){
        cout << i << " " ;
	}
	return pred;
}

// ����
float RandomForest::score(const vector<float> &y_pred, vector<float> y_true){
    float cal = 0;
    for(int i = 0; i< y_pred.size(); i++){
        cal += pow(y_pred[i] - y_true[i], 2);
    }
    cout << cal << endl;
    return cal;
}

