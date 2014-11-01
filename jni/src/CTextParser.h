#ifndef _CTEXTPARSER_H
#define _CTEXTPARSER_H

#include <string.h>
#include <malloc.h>
#include <iostream>
#include <vector>
 
using namespace std;

#ifdef _MSC_VER
	#pragma warning( disable : 4244 ) // conversion from 'const double' to 'float', possible loss of data
	#pragma warning( disable : 4996 ) // '*' was declared deprecated
#endif

#ifdef __INTEL_COMPILER
	#pragma warning(disable: 1786) // function strcpy was declared deprecated ...
	#pragma warning(disable: 279)
	#pragma warning(disable: 1478) //  function "alutLoadWAVFile" was declared "deprecated"
#endif

class CTextParser{
	public:
		CTextParser();
		~CTextParser();

		static char* getFirstWord(const char* const line);
		static char* getSecondWord(const char* const line); 
		static char* getThirdWord(const char* const line);
		static char* getFourthWord(const char* const line);
		static char* getFifthWord(const char* const line);
		static char* getSixthWord(const char* const line);
		static char* getSeventhWord(const char* const line);
		static char* getEightWord(const char* const line);
		static char* getNinthWord(const char* const line);

		static char* getFirstWordWithoutQuotes(const char* const line);
		static char* getSecondWordWithoutQuotes(const char* const line);
		static char* getThirdWordWithoutQuotes(const char* const line);
		static char* getFourthWordWithoutQuotes(const char* const line);
		static char* getFifthWordWithoutQuotes(const char* const line);
		static char* getSixthWordWithoutQuotes(const char* const line);
		static char* getSeventhWordWithoutQuotes(const char* const line);
		static char* getEightWordWithoutQuotes(const char* const line);
		static char* getNinthWordWithoutQuotes(const char* const line);
		static char* getTenthWordWithoutQuotes(const char* const line);
		static char* getEleventhWordWithoutQuotes(const char* const line);

		static void eliminateEmptySpaceBeforeContent(char* line);
		static void eliminateEmptySpaceAfterContent(char* line);
		static void eliminateReturnAtEndOfLine(char* line);
		static void eliminateAllButNumbers(char* line);
		static void eliminateAlphaAndNumericWords(char* line);
		static void eliminateWord(char* line, int wordNr);
		static void eliminateThisCharacter(char* line, char c);
		static void eliminateFromStartToEnd(char* line, int start, int end);
		
		// assuming /* text */ in one line
		static void eliminateOldComment(char* line);

		static void convertSlashToBackslash(char* line);   // prefered over other way around
    static void convertBackslashToSlash(char* line);

		static bool anyWhereEndOfMultiLineComment(const char* const line);
		static bool anyWhereStartOfMultiLineComment(const char* const line);

		static int countNumberOfThisCharacter(const char* line, char c);
		static int getFirstPositionOfThisCharacter(const char* line, char c);
		static int getLastPositionOfThisCharacter(const char* line, char c);

		static char* replaceWord(char* oldWord, char* newWord, const char* const line);
    static char* replaceWordOnce(char* oldWord, char* newWord, const char* const line);
		static void replaceChar(char oldChar, char newChar, char* line);
		
		// returns at which position the word starts. if not found -1.
		static int isWordInside(const char* const word, const char* const line);

		static char* getWordInLine(int n, const char* const line, bool quotes);

		static char* copyFromToEnd(const char* const line, int from);
		static char* copyFromTo(const char* const line, int from, int to);

		static int stricmpTextParser(const char* string1, const char* string2);

		static void convertToLowerCase(char* line);

	private:		
		static bool isFloatAsString(const char* word);
};
#endif
