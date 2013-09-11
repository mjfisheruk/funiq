#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <tclap/CmdLine.h>

using namespace std;

void setup(int argc, char** argv, vector<string>& lines) {
	
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

int main(int argc, char* argv[]) {

	typedef map< string, vector<string>* > StringListMap;

	try {

		StringListMap matches;
		vector<string> lines(0);
		setup(argc, argv, lines);
		StringListMap::iterator matchIt;
		vector<string>::iterator matchItemIt;

		//For every line in input, compare with every key in the hashmap
		vector<string>::iterator inputIt;
		for(inputIt = lines.begin(); inputIt != lines.end(); ++inputIt) {
		    string line = *inputIt;
		    bool matchFound = false;
		    string matchKey;
		    for(matchIt = matches.begin(); matchIt != matches.end(); ++matchIt) {
		    	pair<string, vector<string>* > matchPair = *matchIt;
		    	vector<string> matchList = *matchPair.second;
		    	for(matchItemIt = matchList.begin(); matchItemIt != matchList.end(); ++matchItemIt) {
		    		string matchItem = *matchItemIt;
		    		if(matchItem == line) {
		    			matchFound = true;
		    			matchKey = matchPair.first;
		    			continue;
		    		}
		    	}
		    }
		    if(matchFound) {
    			vector<string>* v = matches[matchKey];
    			v->push_back(line);
		    } else {	
		    	vector<string>* v = new vector<string>(0);
		    	v->push_back(line);
		    	matches[line] = v;
		    }
		}

		for(matchIt = matches.begin(); matchIt != matches.end(); ++matchIt) {
			pair<string, vector<string>* > matchPair = *matchIt;
			vector<string> v = *matchPair.second;
			for(matchItemIt = v.begin(); matchItemIt != v.end(); ++matchItemIt) {
		    	string matchItem = *matchItemIt;
		    	cout << matchItem << " ";
		    }
		    cout << endl;
		}

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}