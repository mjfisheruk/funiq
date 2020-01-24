#include <algorithm>
#include <functional>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "tclap/CmdLine.h"
#include "funiq/Settings.h"
#include "funiq/Matcher.h"

void parseCommandLine(int argc, char** argv, std::string& filename, Settings& settings) {
	
	TCLAP::CmdLine cmd("funiq - Fuzzy Unique Filtering", ' ', "0.3.0");
	
	TCLAP::UnlabeledValueArg<std::string> filenameArg (
		"filename",
		"File to read. If omitted will read from stdin.",
		false, "", "filename");
	TCLAP::ValueArg<float> distanceArg(
		"d","distance",
		"Maximum distance threshold between two strings to be considered duplicates.\n"
		"For the default Levenshtein comparison method, it is the maximum edit distance "
		"allowed for two strings to be considered duplicates.\n"
		"For the Normalized Levenshtein comparison method, it is a number between 0.0 and 1.0 "
		"representing 0% and 100% similarity respectively.",
		false, 3, "number");
	TCLAP::SwitchArg caseSwitch(
		"i","case-insensitive",
		"When active, case differences do not contribute to edit distance.");
	TCLAP::SwitchArg showAllSwitch(
		"a","show-all",
		"Will show all found duplicates");
	TCLAP::SwitchArg showTotalsSwitch(
		"c","show-counts",
		"Precede each output line with the count of the number of times the line occurred"
		"in the input, followed by a single space.");
	TCLAP::SwitchArg ignoreNonAlphaNumericSwitch(
		"I","ignore-non-alpha-numeric",
		"When active, non-alphanumeric characters do not contribute to edit distance.");

	std::vector<std::string> allowedComparisonMethods;
	allowedComparisonMethods.push_back("levenshtein");
	allowedComparisonMethods.push_back("normalized-levenshtein");
	TCLAP::ValuesConstraint<std::string> comparisonMethodsConstraint(allowedComparisonMethods);

	TCLAP::ValueArg<std::string> comparisonMethodArg(
			"m","method",
			"The method used to compare similarity of strings. Defaults to 'levenshtein'",
			false, "levenshtein", &comparisonMethodsConstraint);
	
	cmd.add(filenameArg);
	cmd.add(comparisonMethodArg);
	cmd.add(distanceArg);
	cmd.add(caseSwitch);
	cmd.add(showAllSwitch);
	cmd.add(showTotalsSwitch);
	cmd.add(ignoreNonAlphaNumericSwitch);
	cmd.parse(argc, argv);

	settings.maxDistance = distanceArg.getValue();
	settings.caseInsensitive = caseSwitch.getValue();
	settings.showAllMatches	= showAllSwitch.getValue();
	settings.showTotals = showTotalsSwitch.getValue();
	settings.ignoreNonAlphaNumeric = ignoreNonAlphaNumericSwitch.getValue();

	std::string comparisonMethod = comparisonMethodArg.getValue();
	if(comparisonMethod == "levenshtein") settings.comparisonMethod = Levenshtein;
	if(comparisonMethod == "normalized-levenshtein") settings.comparisonMethod = NormalizedLevenshtein;

	filename = filenameArg.getValue();	
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
