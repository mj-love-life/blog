#include "utils.h"

char util_log[256];


string now(void){
    struct tm t;
    time_t n;
	time(&n);
	localtime_s(&t, &n);
	char result[32];
	strftime(result, sizeof(result), "%Y-%m-%d %H:%M:%S", &t);
	return result;
}

void log(const char * log_msg) {
    printf("[%s] %s \n", now().c_str(), log_msg);
}

int rand(const int begin, const int end){
    LARGE_INTEGER seed;
    QueryPerformanceFrequency(&seed);
    QueryPerformanceCounter(&seed);
    srand(seed.QuadPart);
	return rand() % (end - begin) + begin;
}

void shuffle(vector<int> &cards){
    LARGE_INTEGER seed;
    QueryPerformanceFrequency(&seed);
    QueryPerformanceCounter(&seed);
    srand(seed.QuadPart);
	int n = cards.size();
	for (int i = 0; i < n; ++i) {
		int index = rand() % (n - i) + i;
		if (index != i) {
			int tmp = cards[i];
			cards[i] = cards[index];
			cards[index] = tmp;
		}
	}
}

Sample deal_train_line(const string &line, int feature_num){
    stringstream ss(line);
    Sample sample;
    sample.x.resize(feature_num);
    string str;
    int index = 0;
    while (getline(ss, str, ',')){
        istringstream ss_temp(str);
        if (index <= 12) {
            ss_temp >> sample.x[index];
        }
        else{
            ss_temp >> sample.y;
        }
        index++;
    }
//     for(int i = 0; i < sample.x.size(); i++){
//        cout << sample.x[i] << ' ';
//    }
//     cout << sample.y << endl;
    return sample;
}

vector<Sample> read_train(const char *path, int feature_num) {
    ifstream open_file(path, ios::in);
    string line;
    vector<Sample> trains;
//    int count = 0;
    while ( getline(open_file, line) ) {
        Sample sample = deal_train_line(line, feature_num);
        trains.push_back(sample);
//        count++;
    }
//    printf("%s %d train samples are read",util_log, count);
//    log(util_log);
    open_file.close();
    return trains;
}

vector<float> deal_test_line(const string& line, int feature_num){
    stringstream ss(line);
//    cout << line << endl;
    vector<float> X;
    X.resize(feature_num);
    string str;
    int index = 0;
    while (getline(ss, str, ',')){
        istringstream ss_temp(str);
        ss_temp >> X[index];
        index++;
    }
    return X;
}

vector< vector<float> > read_test(const char *path, int feature_num){
    ifstream open_file(path, ios::in);
    string line;
    vector< vector<float> > tests;
//    int count = 0;
    while ( getline(open_file, line) ) {
        vector<float> test = deal_test_line(line, feature_num);
        tests.push_back(test);
//        count++;
    }
//    printf("%s %d train samples are read",util_log, count);
//    log(util_log);
    open_file.close();
    return tests;
}


vector<float> read_label(const char *path){
    ifstream open_file(path, ios::in);
    string line;
    vector<float> labels;
    float temp = 0.0;
    while ( getline(open_file, line) ) {
        istringstream ss_temp(line);
        ss_temp >> temp;
        labels.push_back(temp);
    }
    // 此处实现完成了
//    printf("%d", labels.size());
//    for(int i = 0; i < labels.size(); i++){
//        printf("%f\n", labels[i]);
//    }
    open_file.close();
    return labels;
}


float calculate_variance(const vector<Sample*> &samples) {
	int n = samples.size();
    float sum = 0;
	for (const auto &sample: samples){
        sum += sample->y;
	}
//	printf("%f\n", sum);
	float mean = (float) sum / n;
	float variance = 0.0;
    for (const auto &sample: samples){
        variance += pow(sample->y, 2);
    }
//    printf("%f\n", mean);
//    printf("%f\n", variance);
	return variance;
}


void write_predict(const vector<float> &test_y, const char *path){

}
