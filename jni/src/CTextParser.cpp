#include "CTextParser.h"

CTextParser::CTextParser(){
}

CTextParser::~CTextParser(){
}

char* CTextParser::getFirstWord(const char* const line){
	return getWordInLine(1, line, false);
}

char* CTextParser::getSecondWord(const char* const line){
	return getWordInLine(2, line, false);
}

char* CTextParser::getThirdWord(const char* const line){
	return getWordInLine(3, line, false);
}

char* CTextParser::getFourthWord(const char* const line){
	return getWordInLine(4, line, false); 
}

char* CTextParser::getFifthWord(const char* const line){
	return getWordInLine(5, line, false);
} 

char* CTextParser::getSixthWord(const char* const line){
	return getWordInLine(6, line, false);
}

char* CTextParser::getSeventhWord(const char* const line){
	return getWordInLine(7, line, false);
}

char* CTextParser::getEightWord(const char* const line){
	return getWordInLine(8, line, false);
}

char* CTextParser::getNinthWord(const char* const line){
	return getWordInLine(9, line, false);
}

char* CTextParser::getFirstWordWithoutQuotes(const char* const line){
	return getWordInLine(1, line, true);
}

char* CTextParser::getSecondWordWithoutQuotes(const char* const line){
	return getWordInLine(2, line, true);
}

char* CTextParser::getThirdWordWithoutQuotes(const char* const line){
	return getWordInLine(3, line, true);
}

char* CTextParser::getFourthWordWithoutQuotes(const char* const line){
	return getWordInLine(4, line, true);
}

char* CTextParser::getFifthWordWithoutQuotes(const char* const line){
	return getWordInLine(5, line, true);
}

char* CTextParser::getSixthWordWithoutQuotes(const char* const line){
	return getWordInLine(6, line, true);
}

char* CTextParser::getSeventhWordWithoutQuotes(const char* const line){
	return getWordInLine(7, line, true);
}

char* CTextParser::getEightWordWithoutQuotes(const char* const line){
	return getWordInLine(8, line, true);
}

char* CTextParser::getNinthWordWithoutQuotes(const char* const line){
	return getWordInLine(9, line, true);
}

char* CTextParser::getTenthWordWithoutQuotes(const char* const line){
	return getWordInLine(10, line, true);
}

char* CTextParser::getEleventhWordWithoutQuotes(const char* const line){
	return getWordInLine(11, line, true);
}


char* CTextParser::getWordInLine(int n, const char* const line, bool quotes){
	if(n <= 0)
		return NULL;
	// first copy the string, because strtok() destroys the original one
	char* duplicat = new char[strlen(line)+1];
	strcpy(duplicat, line);

	char* returnMe;
	char* temp;
	if(!quotes)
		returnMe = strtok(duplicat, " \t\n");
	else
		returnMe = strtok(duplicat, " \t\n\"");

	for(int i = 1; i < n; i++){
		if(!quotes)
			returnMe = strtok(NULL, " \t\n");
		else
			returnMe = strtok(NULL, " \t\n\"");
		if(!returnMe)
			break;
	}

	if(returnMe){
		// copy all characters
		temp = new char[strlen(returnMe)+1];
		strcpy(temp, returnMe);
		// change pointer
		returnMe = temp;
	}

	delete [] duplicat;

	return returnMe;	
}

void CTextParser::eliminateEmptySpaceBeforeContent(char* line){
	const int length = (int)strlen(line);
	char* buffer = new char[length+1];

	int i = 0;
	while(line[i] == ' ' || line[i] == '\t' || line[i] == 0x0A)
		i++;
	
	for(int j = 0; i < length + 1; i++, j++)
		buffer[j] = line[i];

	strcpy(line, buffer);
	delete [] buffer;
}

void CTextParser::eliminateEmptySpaceAfterContent(char* line){
	const int length = (int)strlen(line);

	for(int i = 1; i < length; i++){
		char importantCharacter = line[length - i ];
		if(importantCharacter == ' ' || importantCharacter == '\t' || importantCharacter == 0x0A){
				line[length - i] = 0;
		} else {
			return;
		}
	}
}

void CTextParser::eliminateReturnAtEndOfLine(char* line){
	const int length = (int)strlen(line);
	if (!length)
		return;

	if(length >= 2){
		if( (line[length-2] == 0x0D) && (line[length-1] == 0x0A) ){
			line[length-2] = '\0';
			line[length-1] = '\0';
		}
	}

	if((line[length-1] == 0x0A) || (line[length-1] == 0x0D))
		line[length-1] = '\0';
}

void CTextParser::convertSlashToBackslash(char* line){
	if(!line){
		cout << "CTextParser::convertSlashToBackslash: line = NULL" << endl;
		return;
	}
		
	const int length = (int)strlen(line);

	if (!length)
		return;

	for(int i = 0; i < length; i++){
		if(line[i] == '/')
			line[i] = '\\';
	}
}

void CTextParser::convertBackslashToSlash(char* line){
	if(!line){
		cout << "CTextParser::convertBackslashToSlash: line = NULL" << endl;
		return;
	}
		
	const int length = (int)strlen(line);

	if (!length)
		return;

	for(int i = 0; i < length; i++){
		if(line[i] == '\\')
			line[i] = '/';
	}
}

void CTextParser::convertToLowerCase(char* line){
	const int length = (int)strlen(line);

	for(int i = 0; i < length; i++){
		if( (line[i] >= 65) && (line[i] <= 90) )
			line[i] += 32;
	}
}



int CTextParser::countNumberOfThisCharacter(const char* line, char c){
	const int length = (int)strlen(line);

	int totalNumber = 0;
	for(int i = 0; i < length; i++){
		if(line[i] == c)
			totalNumber++;
	}

	return totalNumber;
}

void CTextParser::eliminateAllButNumbers(char* line){
	const int length = (int)strlen(line);

	for(int i = 0; i < length; i++){
		// 0-9 in ASCII is from 48 to 57.
		// . is used for float, like 1.000
		// - is also okay
		if( ((line[i] < 48) || (line[i] > 57)) && (line[i] != 46) && (line[i] != 45))
			line[i] = ' ';
	}
}

void CTextParser::eliminateAlphaAndNumericWords(char* line){
	vector<int> words;
	for(int i = 1;  ; i++){
		char* newValueString = CTextParser::getWordInLine(i, line, false);

		if(newValueString){
			const int length = (int)strlen(newValueString);
			bool alpha = false;
			bool numeric = false;

			for(int j = 0; j < length; j++){
				// 0-9
				if( (newValueString[j] >= 48) && (newValueString[j] <= 57) )
					numeric = true;
				// a-z and A-Z
				else if( ((newValueString[j] >= 65) && (newValueString[j] < 91)) ||
					     ((newValueString[j] >= 97) && (newValueString[j] < 123)) )
						 alpha = true;
			}	

			// special case: 0.6909f with an 'f' at the end.
			const bool isFloat = isFloatAsString(newValueString);
			delete [] newValueString;

			// remember this word for deleting later
			if (alpha && numeric && !isFloat) 
				words.push_back(i);
				
		} else
			break;
	}	

	
	// now we delete the marked words, from back to front
	int wordsToDelete = (int)words.size();
	for(int k = 0; k < wordsToDelete; k++){
		eliminateWord(line, words[wordsToDelete - k - 1]);
	}
}

bool CTextParser::isFloatAsString(const char* word){
	const int length = (int)strlen(word);

	if(length < 2)
		return false;

	bool lastCharacterIsF = false;
	if(word[length - 1] == 'f')
		lastCharacterIsF = true;
	
	bool allOtherCharactersAreNumbers = true;
	for(int i = 0; i < length - 1; i++){
		// 0-9
		if( !(    ( (word[i]  >= 48) && (word[i]  <= 57)) || (word[i] == '.')) )
			allOtherCharactersAreNumbers = false;
	}

	return (lastCharacterIsF && allOtherCharactersAreNumbers);
}

void CTextParser::eliminateWord(char* line, int wordNr){
	if(wordNr <= 0){
		cout << "CTextParser::eliminateWord: wordNr <= 0!" << endl;
		return;
	}

	if(!line){
		cout << "CTextParser::eliminateWord: line = NULL" << endl;
		return;
	}

	const int lineLength = (int)strlen(line);

	int wordsCounted = 0;

	int startOfRelevantWord = 0;
	int endOfRelevantWord = 0;

	bool wordStarted = false;
	for(int i = 0; i < lineLength; i++){
		// if we aren't in a word right now, there can be free spaces
		// which we ignore or there could be something else, which woul
		// be the next word.
		if(!wordStarted){
			if(line[i] == ' ' || line[i] == '\t')
				continue;
			else {
				wordsCounted++;
				if(wordsCounted == wordNr)
					startOfRelevantWord = i;

				wordStarted = true;
			}
		// if we are in between a word and discover free spaces the
		// word has ended there.
		} else {
			if(line[i] == ' ' || line[i] == '\t')
				wordStarted = false;
				if(wordsCounted == wordNr)
					endOfRelevantWord = i;
		}
	}

	// now clear the word with spaces
	for(int j = startOfRelevantWord; j < endOfRelevantWord; j++)
		line[j] = ' ';	
}

bool CTextParser::anyWhereEndOfMultiLineComment(const char* const line){
	const int length = (int)strlen(line);

	if(length < 2)
		return false;
	for(int i = 0; i < length - 1; i++){
		if( (line[i] == '*') && (line[i+1] == '/') )
			return true;
	}

	return false;
}

bool CTextParser::anyWhereStartOfMultiLineComment(const char* const line){
	const int length = (int)strlen(line);

	if(length < 2)
		return false;
	for(int i = 0; i < length - 1; i++){
		if( (line[i] == '/') && (line[i+1] == '*') )
			return true;
	}

	return false;
}

void CTextParser::eliminateOldComment(char* line){
	const int length = (int)strlen(line);

	// /* and */ take in 4 characters
	if(length < 4)
		return;

	bool oldComment = false;
	for(int i = 0; i < length; i++){
		if(line[i] == '/' && (line[i+1] == '*')){
			oldComment = true;
		}


		if(oldComment) {
			if((line[i] == '*') && (line[i + 1] == '/')){
				oldComment = false;
				line[i] = line[i + 1] = ' ';
			}
		}

		if(oldComment)
			line[i] = ' ';
	}

}

int CTextParser::getFirstPositionOfThisCharacter(const char* line, char c){
	const int length = (int)strlen(line);

	for(int i = 0; i < length; i++){
		if(line[i] == c)
			return i;
	}

//	cout << "CTextParser::getFirstPositionOfThisCharacter - not found: " << c << endl;
	return 0;
}

int CTextParser::getLastPositionOfThisCharacter(const char* line, char c){
	const int length = (int)strlen(line);

	for(int i = length - 1; i > 0; i--){
		if(line[i] == c)
			return i;
	}

	cout << "CTextParser::getLastPositionOfThisCharacter - not found: " << c << endl;
	return 0;
}


void CTextParser::eliminateThisCharacter(char* line, char c){
	const int length = (int)strlen(line);

	for(int i = 0; i < length; i++){
		if(line[i] == c)
			line[i] = ' ';
	}

}

char* CTextParser::replaceWord(char* oldWord, char* newWord, const char* const line){
	char* newLine = new char[strlen(line) + 1];
	strcpy(newLine, line);

  const int oldWordLength = (int)strlen(oldWord);
  const int newWordLength = (int)strlen(newWord);
  const int tempLength = (int)strlen(line) + 1;
	
	while(1){
		int wordPosition = isWordInside(oldWord, newLine);
		
		if(wordPosition == -1)
			break;

    if(wordPosition > (int)strlen(newLine))
      return newLine;


		char* newLineSecondPart = new char[tempLength];

		strcpy(newLineSecondPart, &newLine[wordPosition + oldWordLength]);

		char* convertedNewLine = new char[strlen(newLine) + newWordLength + 1];
		memset(convertedNewLine, 0, (int)strlen(newLine) + newWordLength + 1);
		
		// copy first part
		strncpy(convertedNewLine, newLine, wordPosition);

		// add the new word
		strcat(convertedNewLine, newWord);

		// add the second part
		strcat(convertedNewLine, newLineSecondPart);

		delete [] newLineSecondPart;
		delete [] newLine;

		newLine = convertedNewLine;		
	}

	return newLine;
}

char* CTextParser::replaceWordOnce(char* oldWord, char* newWord, const char* const line){
	char* newLine = new char[strlen(line) + 1];
	strcpy(newLine, line);
	
	int wordPosition = isWordInside(oldWord, newLine);
	
	if(wordPosition == -1)
		return newLine;

  if(wordPosition > (int)strlen(newLine))
    return newLine;

	int oldWordLength = (int)strlen(oldWord);
	int newWordLength = (int)strlen(newWord);

	int tempLength = (int)strlen(line) + 1;
	char* newLineSecondPart = new char[tempLength];

	strcpy(newLineSecondPart, &newLine[wordPosition + oldWordLength]);

	char* convertedNewLine = new char[strlen(newLine) + newWordLength + 1];
	memset(convertedNewLine, 0, (int)strlen(newLine) + newWordLength + 1);
	
	// copy first part
	strncpy(convertedNewLine, newLine, wordPosition);

	// add the new word
	strcat(convertedNewLine, newWord);

	// add the second part
	strcat(convertedNewLine, newLineSecondPart);

	delete [] newLineSecondPart;
	delete [] newLine;

	newLine = convertedNewLine;		

	return newLine;
}

int CTextParser::isWordInside(const char* const word, const char* const line){
	int lineLength = (int)strlen(line);
	int wordLength = (int)strlen(word);

	if(wordLength > lineLength)
		return -1;

	
	bool wordIsInside = false;
	int i;
	for(i = 0; i <= lineLength - wordLength; i++){
		wordIsInside = true;

		// is there any character different?
		for(int j = 0; j < wordLength; j++){
			if(line[i + j] != word[j]){
				wordIsInside = false;
				break;
			}
		}

		// no character was different, we found the word!!
		if(wordIsInside)
			break;
	}

	if(wordIsInside)
		return i;
	else
		return -1;
}

void CTextParser::eliminateFromStartToEnd(char* line, int start, int end){
	const int lineLength = (int)strlen(line);
	if(start > lineLength){
		cout << "CTextParser::eliminateFromStartToEnd: start > lineLength" << endl;
		return;
	}

	if(end > lineLength){
		cout << "CTextParser::eliminateFromStartToEnd: end > lineLength" << endl;
		return;
	}

	for(int i = start; i <= end; i++){
		line[i] = ' ';
	}

}

void CTextParser::replaceChar(char oldChar, char newChar, char* line){
	const int length = (int)strlen(line);

	for(int i = 0; i < length; i++){
		if(line[i] == oldChar)
			line[i] = newChar;
	}
}

int CTextParser::stricmpTextParser(const char* string1, const char* string2){
	if(!string1 || !string2)
		return 1;

	char c1, c2;

	const int length1 = (int)strlen(string1);
	const int length2 = (int)strlen(string2);

	if(length1 > length2)
		return 1;
	if(length1 < length2)
		return -1;

	for(int i = 0;  ; i++){
		c1 = string1[i];
		c2 = string2[i];

		if(!c1)
			return 0;

		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z') {
				c1 -= ('a' - 'A');
			}
			if (c2 >= 'a' && c2 <= 'z') {
				c2 -= ('a' - 'A');
			}
			if (c1 != c2) {
				return c1 < c2 ? -1 : 1;
			}
		}
	}

	return 0;
}

char* CTextParser::copyFromToEnd(const char* const line, int from){
	if(!line)
		return NULL;

	const int length = (int)strlen(line);
	if(from > length - 1)
		return NULL;

	const int newLength = length - from;

	char* newLine = new char[newLength + 1];
	// <= so the final \0 is copied, too.
	for(int i = from; i <= length; i++){
		newLine[i - from] = line[i];
	}

	return newLine;
}

char* CTextParser::copyFromTo(const char* const line, int from, int to){
	if(!line)
		return NULL;

	const int length = (int)strlen(line);
	if(from > length - 1)
		return NULL;

	if(to > length -1)
		return NULL;

	if(to <= from)
		return NULL;

	const int newLength = to - from;

	char* newLine = new char[newLength + 1];

	int i;
	for(i = from; i < to; i++){
		newLine[i - from] = line[i];
	}

	newLine[i - from] = '\0';

	return newLine;
}