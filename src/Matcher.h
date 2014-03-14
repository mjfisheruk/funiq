#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <fstream>
#include <map>

typedef std::vector<std::string> StringList;
typedef std::map< std::string, StringList* > StringListMap;

class Matcher {
	StringList* lines;
	StringListMap* matchMap;
	FuniqSettings* settings;
public:
	Matcher(FuniqSettings& settings);
	~Matcher();
	void add(std::string& line);
	void show(std::ostream &output);
private:
	unsigned int levenshteinDistance(const std::string& s1, const std::string& s2);
	void lowercase(std::string& s);
	bool nonAlphaNumeric(char c);
	void removeNonAlphaNumeric(std::string& s);
};

Matcher::Matcher(FuniqSettings& settings) {
	this->settings = &settings;
	lines = new StringList();
	matchMap = new StringListMap();
}

Matcher::~Matcher() {
	delete lines;
	delete matchMap;
}

void Matcher::add(std::string& line) {
	bool matchFound = false;
	std::string formattedLine = line;
	if(settings->caseInsensitive) this->lowercase(formattedLine);
	if(settings->ignoreNonAlphaNumeric) removeNonAlphaNumeric(formattedLine);

	for(auto matchPair : *matchMap) {
	
		std::string key = matchPair.first;
		if(settings->caseInsensitive) lowercase(key);
		if(settings->ignoreNonAlphaNumeric) removeNonAlphaNumeric(key);
		StringList* matchList = matchPair.second;	
		if(levenshteinDistance(formattedLine, key) <= settings->maxEditDistance) {
			matchFound = true;
			key = matchPair.first;
			matchList->push_back(line);
			continue;
		}
	}
	if(!matchFound) {
		StringList* matchList = new StringList(0);
		matchList->push_back(line);
		(*matchMap)[line] = matchList;
	}
}

void Matcher::show(std::ostream &output) {
	
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

void Matcher::lowercase(std::string& s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}

bool Matcher::nonAlphaNumeric(char c) {
	return !std::isalnum(c);
}

void Matcher::removeNonAlphaNumeric(std::string& s) {
	s.erase(std::remove_if(s.begin(), s.end(), &Matcher::nonAlphaNumeric), s.end());
}