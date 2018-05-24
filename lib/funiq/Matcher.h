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
	unsigned int levenshteinDistance(const std::string& s1, const std::string& s2);
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
		if(levenshteinDistance(normalizedLine, normalizedKey) <= _settings.maxEditDistance) {
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

void Matcher::lowercase(std::string& s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void Matcher::removeNonAlphaNumeric(std::string& s) {
	s.erase(std::remove_if(s.begin(), s.end(), [](const char& c){
                return !std::isalnum(c);
              }), s.end());
}

unsigned int Matcher::levenshteinDistance(const std::string& s1, const std::string& s2) {
	
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

#endif