#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <tclap/CmdLine.h>

using namespace std;

typedef vector<string> StringList;
typedef map< string, StringList* > StringListMap;
typedef pair<string, vector<string>* > StringListPair;

void setup(int argc, char** argv, StringList& lines) {
	
	TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.1");
	TCLAP::UnlabeledValueArg<string> filenameArg
		("filename", "Optional file to read. If omitted will read from stdin.",
		false, "", "filename");
	
	cmd.add(filenameArg);
	cmd.parse(argc, argv);

	istream* inputStream;

	string filename = filenameArg.getValue();

	if(filename == "") {
		inputStream = &cin;
	} else {
		inputStream = new ifstream(filename.c_str());
	}

	for (string line; getline(*inputStream, line); ) {
		lines.push_back(line);
	}
}

void buildMap(StringList& lines, StringListMap& matchMap) {

	StringList::iterator linesIt;
	StringListMap::iterator matchMapIt;

	for(linesIt = lines.begin(); linesIt != lines.end(); ++linesIt) {
	    bool matchFound = false;
	    string line = *linesIt;
	    
	    for(matchMapIt = matchMap.begin(); matchMapIt != matchMap.end(); ++matchMapIt) {
	    	
	    	StringListPair matchPair = *matchMapIt;	
	    	string key = matchPair.first;
	    	StringList* matchList = matchPair.second;	
    		
    		if(line == key) {
    			matchFound = true;
    			key = matchPair.first;
    			matchList->push_back(line);
    			continue;
    		}
	    }

	    if(!matchFound) {
	    	StringList* matchList = new StringList(0);
	    	matchList->push_back(line);
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
		
		StringListMap matchMap;
		vector<string> lines(0);
		setup(argc, argv, lines);
		buildMap(lines, matchMap);
		displayResults(matchMap);

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}