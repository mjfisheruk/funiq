#ifndef _FUNIQ_SETTINGS_
#define _FUNIQ_SETTINGS_

enum ComparisonMethod { Levenshtein, NormalizedLevenshtein };

struct Settings
{
	Settings() :
		maxDistance(3),
		caseInsensitive(false),
        comparisonMethod(Levenshtein),
		showAllMatches(false),
		showTotals(false),
		ignoreNonAlphaNumeric(false)
		{}

	float maxDistance;
	bool caseInsensitive;
	ComparisonMethod comparisonMethod;
	bool showAllMatches;
	bool showTotals;
	bool ignoreNonAlphaNumeric;
	int totalsFieldWidth;
};

#endif
