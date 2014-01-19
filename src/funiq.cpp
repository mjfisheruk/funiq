#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "tclap/CmdLine.h"
#include "funiq/Settings.h"
#include "funiq/Matcher.h"

void parseCommandLine(int argc, char** argv, std::string& filename, Settings& settings) {
	
	TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.1");
	
	TCLAP::UnlabeledValueArg<std::string> filenameArg (
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

void readLines(const std::string& filename, StringList& lines) {
	std::istream* inputStream;
	if(filename == "") {
		inputStream = &std::cin;
	} else {
		inputStream = new std::ifstream(filename.c_str());
	}

	for (std::string line; getline(*inputStream, line); ) {
		lines.push_back(line);
	}
}

unsigned int levenshteinDistance(const std::string& s1, const std::string& s2) {
	
	unsigned int len1 = s1.size();
	unsigned int len2 = s2.size();
	std::vector<unsigned int> col(len2+1);
	std::vector<unsigned int> prevCol(len2+1);

	for (unsigned int i = 0; i < prevCol.size(); i++) {
		prevCol[i] = i;
	}
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++) {
			col[j+1] = std::min(
				std::min( 1 + col[j], 1 + prevCol[1 + j]),
				prevCol[j] + (s1[i]==s2[j] ? 0 : 1)
			);
		}
		col.swap(prevCol);
	}
	return prevCol[len2];
}

void lowercase(std::string& s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void buildMap(StringList& lines, StringListMap& matchMap, const Settings& settings) {

	for(std::string originalLine : lines) {
		bool matchFound = false;

		std::string line = originalLine;
		if(settings.caseInsensitive) lowercase(line);

		for(auto matchPair : matchMap) {

			std::string key = matchPair.first;
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

void displayResults(StringListMap& matchMap, Settings& settings) {
	
	for(auto matchPair : matchMap) {
		StringList v = *matchPair.second;
		bool first = true;
		for(std::string matchItem : v) {
			if(first || settings.showAllMatches) {
				if(!first) std::cout << "\t";
				std::cout << matchItem;
				first = false;
			}
		}
		std::cout << std::endl;
	}
}





std::istream* getInput(const std::string& filename) {
	std::istream* inputStream;
	if(filename == "") {
		inputStream = &std::cin;
	} else {
		inputStream = new std::ifstream(filename.c_str());
	}
	return inputStream;
}

int main(int argc, char* argv[]) {
	
	try {

		std::string filename;
		Settings settings;
		parseCommandLine(argc, argv, filename, settings);
		
		Matcher matcher(settings);
		std::istream* inputStream = getInput(filename);		
		for (std::string line; getline(*inputStream, line); ) {
			matcher.add(line);
		}

		matcher.show(&std::cout);

	} catch (TCLAP::ArgException &e) {
		std::cerr << "An error occurred: ";
		std::cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}