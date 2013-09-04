#include <string>
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

	try {

		vector<string> lines(0);
		setup(argc, argv, lines);

		for(vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
		    cout << *it << endl;
		}

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}