#ifndef _FUNIQ_SETTINGS_
#define _FUNIQ_SETTINGS_

struct FuniqSettings
{
	FuniqSettings() :
		maxEditDistance(3),
		caseInsensitive(false),
		showAllMatches(false),
		showTotals(false) {}

	unsigned int maxEditDistance;
	bool caseInsensitive;
	bool showAllMatches;
	bool showTotals;
};

#endif
