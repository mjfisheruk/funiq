#include <iostream>
#include <fstream>
#include <string>
#include <tclap/CmdLine.h>

using namespace std;

int main(int argc, char* argv[]) {

	try {

		TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.1");
		TCLAP::UnlabeledValueArg<string> filenameArg
			("filename", "Optional file to read. If omitted will read from stdin.",
			false, "", "filename");
		
		cmd.add(filenameArg);
		cmd.parse(argc, argv);

		string filename = filenameArg.getValue();
		istream* inputStream;
		ifstream fileStream;

		if(filename == "") {
			inputStream = &cin;
		} else {
			fileStream.open(filename.c_str());
			inputStream = &fileStream;
		}

		for (string line; getline(*inputStream, line); ) {
			cout << "Hi " << line << "!" << endl;
		}

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}