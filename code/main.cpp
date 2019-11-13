#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string readCSVFile(string name) {
	ifstream file;
	string content;
	string line;
	file.open(name);
	while (getline(file, line)) {
		content += line + "\n";
	}
	file.close();
	return content;
}

void parse(string csv, string teams[20], int scores[20][38], int ranks[20][38]) {
	istringstream iss(csv); 
	for (int i = 0; i < 20; i++) {
		string line;
		getline(iss, line, '\n');
		istringstream streamLine(line);
		getline(streamLine, teams[i], ',');
		for (int j = 0; j < 38; j++) {
			string tmp;
			getline(streamLine, tmp, ',');
			ranks[i][j] = stoi(tmp);
			getline(streamLine, tmp, ',');
			scores[i][j] = stoi(tmp);
			for (int k = 0; k < 4; k++) {
				getline(streamLine, tmp, ',');
			}
		}
	}
}

void aff(int scores[20][38]) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			cout << scores[i][j] << " ";
		}
		cout << endl;
	}
}

static const GLfloat g_vertex_buffer_data[] = {
   -0.8f, -0.8f, 0.0f,
   0.8f, -0.8f, 0.0f,
   -0.8f,  0.8f, 0.0f,

   0.8f, 0.8f, 0.0f,
   0.8f, -0.8f, 0.0f,
   -0.8f,  0.8f, 0.0f,

};

string updateRanks(int teams, int ranks[][]){
	for(int i=0; i<20; i++){
		for(int j=0;i<38;j++){
			glDrawArrays(GL_LINES(ranks[i][j],ranks[i][j+1])) 





int main() {
	string rawCSV = readCSVFile("../data/rankspts.csv");
	string teams[20];
	int ranks[20][38];
	int scores[20][38];
	parse(rawCSV, teams, scores, ranks);
	aff(scores);
	return 1;
}
