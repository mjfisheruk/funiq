#include <string>
#include <iostream>
#include <fstream>
#include <tclap/CmdLine.h>

using namespace std;

void setup(int argc, char** argv, istream** inputStream) {
	
	TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.1");
	TCLAP::UnlabeledValueArg<string> filenameArg
		("filename", "Optional file to read. If omitted will read from stdin.",
		false, "", "filename");
	
	cmd.add(filenameArg);
	cmd.parse(argc, argv);

	string filename = filenameArg.getValue();

	if(filename == "") {
		*inputStream = &cin;
	} else {
		*inputStream = new ifstream(filename.c_str());
	}
}

int main(int argc, char* argv[]) {

	try {

		istream* inputStream = NULL;
		setup(argc, argv, &inputStream);

		for (string line; getline(*inputStream, line); ) {
			cout << " - " << line << endl;
		}

	} catch (TCLAP::ArgException &e) {
		cerr << "An error occured: ";
		cerr << e.error() << " for arg " << e.argId() << std::endl;
	}
}