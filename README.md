Funiq (fuzzy uniq) is a command line tool for performing fuzzy string matching against lists of words. To compile, clone the source and run make at the project's root; the funiq binary will be compiled to the bin/ directory.

As an example, consider the file test.txt:

	The Fellowship of The Ring
	the fellowship ofteh reing
	The Flopship of the Rung
	Felloship of the Ring
	The Two Towers
	The Twin Towers
	the towers
	The Return of the King
	Return of the King
	Teh return of theking

Running funiq on this list and telling it to ignore casing (-i) and non-alphanumeric characters (-c)

	$ funiq -ci test.txt

Results in:

	Felloship of the Ring
	The Fellowship of The Ring
	The Flopship of the Rung
	The Return of the King
	The Two Towers

Which is ok, but it hasn't produced the desired results with The Fellowship of the Ring. Using the -d option, we can increase the threshold at which matches are considered (default is 3):

	$ funiq -ci -d 4 test.txt

	The Fellowship of The Ring
	The Return of the King
	The Two Towers
 
Which is what we were looking for.

Funiq can read from a file or have its input piped from stdin.

	USAGE: 

     funiq  [-c] [-t] [-a] [-i] [-d <integer>] [--] [--version] [-h]
                <filename>


	Where: 

	   -I,  --ignore-non-alpha-numeric
	     When active, non-alphanumeric characters do not contribute to edit
	     distance.

	   -c,  --show-counts
	     Precede each output line with the count of the number of times the line occurred
	     in the input, followed by a single space.

	   -a,  --show-all
	     Will show all found duplicates

	   -i,  --case-insensitive
	     When active, case differences do not contribute to edit distance.

	   -d <integer>,  --distance <integer>
	     Maximum edit distance between two strings to be considered a match.
	     Default: 3

	   --,  --ignore_rest
	     Ignores the rest of the labeled arguments following this flag.

	   --version
	     Displays version information and exits.

	   -h,  --help
	     Displays usage information and exits.

	   <filename>
	     File to read. If omitted will read from stdin.


	   funiq - Fuzzy Unique Filtering



