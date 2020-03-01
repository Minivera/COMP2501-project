#pragma once

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class FileUtils {
public:
	static string LoadTextFile(const char *filename);

	// Method that loads an arbitrary level from a CSV file
	static vector<vector<string>> LoadCsvLevel(const char* filename, const string& delimiter);
};