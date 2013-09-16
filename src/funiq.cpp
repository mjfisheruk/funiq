#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tclap/CmdLine.h>

#include "FuniqSettings.h"

using namespace std;

typedef vector<string> StringList;
typedef map< string, StringList* > StringListMap;

void parseCommandLine(int argc, char** argv, string& filename, FuniqSettings& settings) {
	
	TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.1");
	
	TCLAP::UnlabeledValueArg<string> filenameArg (
		"filename",
		"File to read. If omitted will read from stdin.",
		false, "", "filename");
	TCLAP::ValueArg<unsigned int> distanceArg(
		"d","distance",
		"Maximum edit distance between two strings to be considered a match.",
		false, 3, "integer");
	TCLAP::SwitchArg caseSwitch(
		"i","case-insensitive",
		"When active, case differences do not contribute to edit distance.");
	TCLAP::SwitchArg showAllSwitch(
		"a","show-all",
		"Will show all found duplicates");
	
	cmd.add(filenameArg);
	cmd.add(distanceArg);
	cmd.add(caseSwitch);
	cmd.add(showAllSwitch);
	cmd.parse(argc, argv);

	settings.maxEditDistance = distanceArg.getValue();
	settings.caseInsensitive = caseSwitch.getValue();
	settings.showAllMatches	= showAllSwitch.getValue();
	filename = filenameArg.getValue();	
}

void readLines(const string& filename, StringList& lines) {
	istream* inputStream;
	if(filename == "") {
		inputStream = &cin;
	} else {
		inputStream = new ifstream(filename.c_str());
	}

	for (string line; getline(*inputStream, line); ) {
		lines.push_back(line);
	}
}

unsigned int levenshteinDistance(const string& s1, const string& s2) {
	
	unsigned int len1 = s1.size();
	unsigned int len2 = s2.size();
	vector<unsigned int> col(len2+1);
	vector<unsigned int> prevCol(len2+1);

	for (unsigned int i = 0; i < prevCol.size(); i++) {
		prevCol[i] = i;
	}
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++) {
			col[j+1] = min(
				min( 1 + col[j], 1 + prevCol[1 + j]),
				prevCol[j] + (s1[i]==s2[j] ? 0 : 1)
			);
		}
		col.swap(prevCol);
	}
	return prevCol[len2];
}

void lowercase(string& s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void buildMap(StringList& lines, StringListMap& matchMap, const FuniqSettings& settings) {

	for(string originalLine : lines) {
		bool matchFound = false;

		string line = originalLine;
		if(settings.caseInsensitive) lowercase(line);

		for(auto matchPair : matchMap) {

			string key = matchPair.first;
			if(settings.caseInsensitive) lowercase(key);
			StringList* matchList = matchPair.second;	
			
			if(levenshteinDistance(line, key) <= settings.maxEditDistance) {
				matchFound = true;
				key = matchPair.first;
				matchList->push_back(originalLine);
				continue;
			}
		}

		if(!matchFound) {
			StringList* matchList = new StringList(0);
			matchList->push_back(originalLine);
			matchMap[line] = matchList;
		}
	}

}

void displayResults(StringListMap& matchMap, FuniqSettings& settings) {
	
	for(auto matchPair : matchMap) {
		StringList v = *matchPair.second;
		bool first = true;
		for(string matchItem : v) {
			if(first || settings.showAllMatches) {
				if(!first) cout << "\t";
				cout << matchItem;
				first = false;
			}
		}
		cout << endl;
	}
}

int main(int argc, char* argv[]) {
	
	try {

		string filename;
		FuniqSettings settings;
		parseCommandLine(argc, argv, filename, settings);
		
		vector<string> lines(0);
		readLines(filename, lines);
		
		StringListMap matchMap;
		buildMap(lines, matchMap, settings);
		
		displayResults(matchMap, settings);

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}