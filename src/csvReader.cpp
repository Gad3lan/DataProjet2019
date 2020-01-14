#include "csvReader.h"
#include "loadShaders.h"

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

void parse(string csv, string teams[nbTeam], int scores[nbTeam][nbMatch], int ranks[nbTeam][nbMatch]) {
	istringstream iss(csv);
	for (int i = 0; i < nbTeam; i++) {
		string line;
		getline(iss, line, '\n');
		istringstream streamLine(line);
		getline(streamLine, teams[i], ',');
		for (int j = 0; j < nbMatch; j++) {
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

void aff(int scores[nbTeam][nbMatch]) {
	for (int i = 0; i < nbTeam; i++) {
		for (int j = 0; j < nbMatch; j++) {
			cout << scores[i][j] << " ";
		}
		cout << endl;
	}
}