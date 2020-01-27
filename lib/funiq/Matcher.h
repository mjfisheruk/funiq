#ifndef _FUNIQ_MATCHER_
#define _FUNIQ_MATCHER_

#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <cctype>
#include <map>

#include "Settings.h"
#include "similarity.h"

#define TOTALS_FIELD_WIDTH 7 // count-field width 7 used by GNU uniq

typedef std::vector<std::string> StringList;
typedef std::map< std::string, StringList* > StringListMap;


class Matcher{
public:
	Matcher(Settings& settings);
	~Matcher();
	void add(std::string line);
	void show(std::ostream* line);
private:
	Settings& _settings;
	StringListMap* matchMap;
	void lowercase(std::string& s);
	void removeNonAlphaNumeric(std::string& s);
	bool isMatch(const std::string& s1, const std::string& s2);
};

Matcher::Matcher(Settings& settings):_settings(settings) {
	matchMap = new StringListMap();
}

Matcher::~Matcher() {
	delete(matchMap);
}

void Matcher::add(std::string line) {
	bool matchFound = false;
	std::string normalizedLine = line;
	if(_settings.caseInsensitive) lowercase(normalizedLine);
	if(_settings.ignoreNonAlphaNumeric) removeNonAlphaNumeric(normalizedLine);
	for(auto matchPair : *matchMap) {
		std::string key = matchPair.first;
		std::string normalizedKey = key;
		if(_settings.caseInsensitive) lowercase(normalizedKey);
		if(_settings.ignoreNonAlphaNumeric) removeNonAlphaNumeric(normalizedKey);
		StringList* matchList = matchPair.second;	
		if(isMatch(normalizedLine, normalizedKey)) {
			matchFound = true;
			matchList->push_back(normalizedLine);
			continue;
		}
	}

	if(!matchFound) {
		StringList* matchList = new StringList(0);
		matchList->push_back(line);
		(*matchMap)[line] = matchList;
	}
}

// todo: count max width necessary
void Matcher::show(std::ostream* output) {
	for(auto matchPair : *matchMap) {
		StringList v = *matchPair.second;
		bool first = true;
		for(std::string matchItem : v) {
			if(first || _settings.showAllMatches) {
				if(first && _settings.showTotals)
					*output << 
						std::setw(TOTALS_FIELD_WIDTH) <<
						v.size() << " "; // space for compatibility with GNU uniq
				if(!first) *output << "\t";
				*output << matchItem;
				first = false;
			}
		}
		*output << std::endl;
	}
}

bool Matcher::isMatch(const std::string& s1, const std::string& s2) {
	if (_settings.comparisonMethod == NormalizedLevenshtein) {
		return similarity::normalizedLevenshtein(s1, s2) <= _settings.maxDistance;
	}
	return similarity::levenshteinDistance(s1, s2) <= _settings.maxDistance;
}

void Matcher::lowercase(std::string& s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void Matcher::removeNonAlphaNumeric(std::string& s) {
	s.erase(std::remove_if(s.begin(), s.end(), [](const char& c){
		return !std::isalnum(c);
	}), s.end());
}



#endif