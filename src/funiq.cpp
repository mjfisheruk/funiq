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
typedef pair<string, vector<string>* > StringListPair;

void setup(int argc, char** argv, StringList& lines, FuniqSettings& settings) {
	
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
	
	cmd.add(filenameArg);
	cmd.add(distanceArg);
	cmd.add(caseSwitch);
	cmd.parse(argc, argv);

	settings.maxEditDistance = distanceArg.getValue();
	settings.caseInsensitive = caseSwitch.getValue();
	
	string filename = filenameArg.getValue();

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

	StringList::iterator linesIt;
	StringListMap::iterator matchMapIt;

	for(linesIt = lines.begin(); linesIt != lines.end(); ++linesIt) {
	    bool matchFound = false;
	    string originalLine = *linesIt;

	    string line = originalLine;
	    if(settings.caseInsensitive) lowercase(line);

	    for(matchMapIt = matchMap.begin(); matchMapIt != matchMap.end(); ++matchMapIt) {
	    	
	    	StringListPair matchPair = *matchMapIt;	
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

void displayResults(StringListMap& matchMap) {
	
	StringListMap::iterator matchIt;
	StringList::iterator matchItemIt;

	for(matchIt = matchMap.begin(); matchIt != matchMap.end(); ++matchIt) {
		StringListPair matchPair = *matchIt;
		StringList v = *matchPair.second;
		for(matchItemIt = v.begin(); matchItemIt != v.end(); ++matchItemIt) {
	    	string matchItem = *matchItemIt;
	    	cout << matchItem << "\t";
	    }
	    cout << endl;
	}
}

int main(int argc, char* argv[]) {
	
	try {

		FuniqSettings settings;
		vector<string> lines(0);
		setup(argc, argv, lines, settings);
		
		StringListMap matchMap;
		buildMap(lines, matchMap, settings);
		
		displayResults(matchMap);

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}