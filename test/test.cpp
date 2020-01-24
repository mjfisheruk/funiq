#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <iostream>

#include "catch/catch.hpp"
#include "funiq/Settings.h"
#include "funiq/Matcher.h"

std::string getNextLine(std::stringstream *stream) {
    std::string line;
    std::getline(*stream, line);
    return line;
}

TEST_CASE( "First README example is correct", "[matcher]" ) {
    Settings settings;
    settings.caseInsensitive = true;
    settings.ignoreNonAlphaNumeric = true;

    Matcher matcher(settings);

    matcher.add("The Fellowship of The Ring");
    matcher.add("the fellowship ofteh reing");
    matcher.add("The Flopship of the Rung");
    matcher.add("Felloship of the Ring");
    matcher.add("The Two Towers");
    matcher.add("The Twin Towers");
    matcher.add("the towers");
    matcher.add("The Return of the King");
    matcher.add("Return of the King");
    matcher.add("Teh return of theking");

    std::stringstream output;
    matcher.show(&output);

    REQUIRE(getNextLine(&output) == "Felloship of the Ring");
    REQUIRE(getNextLine(&output) == "The Fellowship of The Ring");
    REQUIRE(getNextLine(&output) == "The Flopship of the Rung");
    REQUIRE(getNextLine(&output) == "The Return of the King");
    REQUIRE(getNextLine(&output) == "The Two Towers");
    REQUIRE(getNextLine(&output) == "");
}

TEST_CASE( "Second README example is correct", "[matcher]" ) {
    Settings settings;
    settings.caseInsensitive = true;
    settings.ignoreNonAlphaNumeric = true;
    settings.maxDistance = 4;

    Matcher matcher(settings);

    matcher.add("The Fellowship of The Ring");
    matcher.add("the fellowship ofteh reing");
    matcher.add("The Flopship of the Rung");
    matcher.add("Felloship of the Ring");
    matcher.add("The Two Towers");
    matcher.add("The Twin Towers");
    matcher.add("the towers");
    matcher.add("The Return of the King");
    matcher.add("Return of the King");
    matcher.add("Teh return of theking");

    std::stringstream output;
    matcher.show(&output);

    REQUIRE(getNextLine(&output) == "The Fellowship of The Ring");
    REQUIRE(getNextLine(&output) == "The Return of the King");
    REQUIRE(getNextLine(&output) == "The Two Towers");
    REQUIRE(getNextLine(&output) == "");
}

TEST_CASE( "Normalised Levenshtein setting for short strings", "[matcher]" ) {
    Settings settings;
    settings.comparisonMethod = NormalizedLevenshtein;

	// _foo -> _ normalized distance is 0.75
	// _foo -> _moo normalized distance is 0.25
    settings.maxDistance = 0.26;

    Matcher matcher(settings);

    matcher.add("_");
    matcher.add("_foo");
    matcher.add("_moo");

    std::stringstream output;
    matcher.show(&output);

    REQUIRE(getNextLine(&output) == "_");
    REQUIRE(getNextLine(&output) == "_foo");
    REQUIRE(getNextLine(&output) == "");
}

TEST_CASE( "Normalised Levenshtein setting for short and long strings", "[matcher]" ) {
	Settings settings;
	settings.comparisonMethod = NormalizedLevenshtein;
	settings.maxDistance = 0.35;

	Matcher matcher(settings);

	// One character difference
	matcher.add("rat");
	matcher.add("cat");

	// Two character difference
	matcher.add("elephant");
	matcher.add("elephenty");

	// All different
	matcher.add("monkey");

	std::stringstream output;
	matcher.show(&output);

	REQUIRE(getNextLine(&output) == "elephant");
	REQUIRE(getNextLine(&output) == "monkey");
	REQUIRE(getNextLine(&output) == "rat");
	REQUIRE(getNextLine(&output) == "");
}
