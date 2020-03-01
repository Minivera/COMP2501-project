#include "FileUtils.h"

string FileUtils::LoadTextFile(const char *filename) {
	// Open file
	ifstream f;
	f.open(filename);
	if (f.fail()) {
		throw(ios_base::failure(string("Error opening file ") + string(filename)));
	}

	// Read file
	string content;
	string line;
	while (getline(f, line)) {
		content += line + "\n";
	}

	// Close file
	f.close();

	return content;
}

vector<vector<string>> FileUtils::LoadCsvLevel(const char* filename, const string& delimiter) {
	// Open file
	ifstream f;
	f.open(filename);
	if (f.fail()) {
		throw(ios_base::failure(string("Error opening file ") + string(filename)));
	}

	vector<vector<string>> output = vector<vector<string>>();

	string line;
	// Get every line in the file
	while (getline(f, line)) {
		// Create the current line
		vector<string> current = vector<string>();

		size_t pos = 0;
		string element;
		// Split the line into delimitters
		while ((pos = line.find(delimiter)) != string::npos) {
			element = line.substr(0, pos);

			current.push_back(element);

			line.erase(0, pos + delimiter.length());
		}

		current.push_back(line);

		// Add the new line in our table of elements
		output.push_back(current);
	}

	return output;
}
