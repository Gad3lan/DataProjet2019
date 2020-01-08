#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string readCSVFile(string name);
void parse(string csv, string teams[20], int scores[20][38], int ranks[20][38]);
void aff(int scores[20][38]);

#endif