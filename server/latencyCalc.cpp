#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

using namespace std;

double avg(const vector<double> &vec) {
	double sum = 0;
	for(int i = 0; i < vec.size(); ++i) {
		sum += vec[i];
	}
	return sum / vec.size();
}

int main() {
	ofstream csSummary("output/csSummary.txt");
	if (!csSummary.is_open()) {
		cout << "Failed to open output/csSummary.txt" << endl;
		exit(1);
	}

	ofstream scSummary("output/scSummary.txt");
	if (!scSummary.is_open()) {
		cout << "Failed to open output/scSummary.txt" << endl;
		exit(1);
	}

	ifstream iFile("output/latency.txt");
	if (!iFile.is_open()) {
		cout << "Failed to open output/latency.txt" << endl;
		exit(1);
	}

	double d1;
	double d2;
	double d3;
	double d4;
	vector<double> csTimes;
	vector<double> scTimes;

	while(iFile >> d1 >> d2 >> d3 >> d4) {
		double diff1 = d2-d1;
		double diff2 = d4-d3;

		csTimes.push_back(diff1);
		scTimes.push_back(diff2);

		csSummary << diff1 << " ms" << endl;
		scSummary << diff2 << " ms" << endl;
	}
	csSummary << endl;
	csSummary << "Average latency from client to server: " << avg(csTimes) << " ms" << endl;

	scSummary << endl;
	scSummary << "Average latency from server to client: " << avg(scTimes) << " ms" << endl;	
}