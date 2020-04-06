// BibleMemory.cpp : This file contains the 'main' function.
//
// Copyright (c) 2020, Loren Burkholder
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <exception>
#include <sstream>
#include "getch.h"

// make sure that the PATH is defined
#if not defined PATH
	#error "You must define the path to the Bible file! Please pass this path to the compiler."
#endif

// global variables
const std::string g_version = "4.1.2";
std::string g_bible;

class BadFileEx : public std::exception
{
private:
	const char *m_what;
public:
	BadFileEx(const char *what) : m_what{ what } {}
	virtual const char* what() { return m_what; }
};

//int _getch();
//int _getche();
void loadBible(const std::string&);
void printHelpMessage();
bool shouldDoAnotherVerse();
void runQuiz(std::iostream&, std::string&);
void getVerseString(std::string&, int, std::string&, std::string &);
void getReference(std::istream&, std::string&);


int main(int argc, char* argv[])
{
	// This holds the filename. The default filename will be "verse.txt". However, this will only
	// be used if the user enters the /f parameter but no filename.
	std::string filename = "verse.txt";
	bool useFile = false; // should we use file input?
	// parse args
	for (int i = 0; i < argc; ++i)
	{
		// give help message
		if (strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "/h") == 0 || strcmp(argv[i], "/H") == 0 || strcmp(argv[i], "/help") == 0 \
			|| strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "--help") == 0)
		{
			printHelpMessage();
			exit(0);
		}
		// filename has been passed to the program
		else if (strcmp(argv[i], "/f") == 0 || strcmp(argv[i], "/F") == 0 || strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "-F") == 0)
		{
			if (argc >= 3) // program name, /f, filename and any other parameters passed
				filename = argv[i + 1]; // copy the user filename into memory
			useFile = true; // We're using a file, so set the indicator
			//bibleLoader.detach(); // we don't need this thread, so let it go
		}
		else if (strcmp(argv[i], "/v") == 0 || strcmp(argv[i], "/V") == 0 || strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "-V") == 0 \
			|| strcmp(argv[i], "--version") == 0)
		{
			std::cout << g_version << "\n";
			exit(0);
		}
	}
	std::cout << "Welcome to the Bible Memory program! (version " << g_version << ")\n"; // print greetings and salutations
	
	// PATH is a macro passed to the preprocessor during compilation.
	std::string g_pathToBible = PATH;
	
	try
	{
		loadBible(g_pathToBible);
	}
	catch (std::exception& ex)
	{
		std::cout << "Error: " << ex.what();
		exit(1);
	}
	catch (...)
	{
		std::cout << "Error: Unknown exception.\n";
		exit(1);
	}

	// versesToDo is the iostream device to read verses from in my format. (The
	// program copies the requested verses into a temporary std::stringstream, and then
	// reads them back. For me, this was simpler than reading in words from
	// an std::string. reference contains the reference requested. anotherVerse is either 'y'
	// or 'n' to indicate whether we should do another verse.
	std::iostream* versesToDo = nullptr;
	std::string reference, oldRef;
	char anotherVerse = '\0';
	do
	{
		try
		{
			if (!useFile)
			{
				versesToDo = new std::stringstream{ " " };
				// get verse to use
				std::cout << "Reference to use: ";
				getReference(std::cin, reference);
				if (reference[0] == '<')
					reference = oldRef;
				// get user preferences on how many verses
				std::cout << "Number of verses to do (starting from the reference): ";
				unsigned int numOfVerses;
				std::cin >> numOfVerses;
				std::string theVerse;
				getVerseString(reference, numOfVerses, g_bible, theVerse);
				*versesToDo << theVerse << "\n-----";
				oldRef = reference;
			}
			else if (useFile)
			{
				versesToDo = new std::fstream{ filename, std::ios::in };
				std::cout << "\nUsing file: " << filename;
				// is it an empty file?
				if (!versesToDo)
					throw BadFileEx{ "Invalid file!" };
			}
			runQuiz(*versesToDo, reference);
		}
		catch (BadFileEx &ex)
		{
			std::cout << "Error: " << ex.what() << "\n";
			exit(-1);
		}
		catch (std::exception &ex)
		{
			std::cout << "Error: " << ex.what() << "\n";
		}
		catch (...)
		{
			std::cout << "Error: Unknown exception.\n";
		}
	} while (!useFile && shouldDoAnotherVerse());
	// we're done, let's quit
	std::cout << "Press any key to exit...";
	_getch();
	std::cout << "\n";
	// delete the verse string pointer
	delete versesToDo;
	return 0;
}

void loadBible(const std::string &filename)
{
	std::fstream bible{ filename.c_str(), std::ios::in };
	if (!bible)
		throw BadFileEx{ "Invalid Bible file!" };
	std::string temp;
	while (std::getline(bible, temp))
		g_bible += temp + '\n';
	bible.close();
}

void printHelpMessage()
{
	std::cout << "You will be asked for a reference. Then you will be asked for the\n"
				 "number of verses to be quizzed on after the starting reference. Then\n"
				 "type the first letter of each word of the verse. For a hint, type \'?\'.\n"
				 "For support, email Loren Burkholder at <computersemiexpert@gmail.com>."
				 "\n\nCommand line usage :\n"
				 "\t/? - Displays this message.\n"
				 "\t/f - Specifies file with verses e.g. /f VerseFile.txt\n"
				 "\t/v - Displays the program version.\n";
}

bool shouldDoAnotherVerse()
{
	char anotherVerse = '\0';
	std::cout << "Do you want to do another verse? ";
	while (anotherVerse != 'n' && anotherVerse != 'y' && anotherVerse != 'Y' && anotherVerse != 'N')
	{
		std::cout << "(y or n) ";
		anotherVerse = _getche();
		std::cout << "\n";
	}
	return (anotherVerse == 'y' || anotherVerse == 'Y') ? true : false;
}

void runQuiz(std::iostream& verseFile, std::string& reference)
{
	// outputting verse reference
	std::cout << "\n\nVerse: ";
	getReference(verseFile, reference);
	std::cout << reference << "\n\n";
	// variables we will need
	std::string nextWord;
	int incorrectTries = 0;
	bool printedHint = false;
	// do the guessing game
	_getch(); // needed to make this work on Linux
	while (verseFile)
	{
		verseFile >> nextWord;
		// end of file
		if (nextWord == "-----")
			break;
		// next verse
		else if (nextWord == "+++++")
		{
			std::cout << "\n\n" << incorrectTries << " incorrect guesses.\n";
			incorrectTries = 0;
			std::cout << "Next verse: ";
			getReference(verseFile, reference);
			std::cout << reference << "\n\n";
			continue;
		}
		// get the guess and print the word if necessary
		char guess = _getch();
		// second condition is basically checking for lowercase-uppercase differences
		while ((guess != nextWord[0]) && (abs('A' - 'a') != abs(guess - nextWord[0])))
		{
			// Does the user want a hint? We'll only print the hint once
			if (guess == '?' && printedHint == false)
			{
				printedHint = true;
				std::cout << nextWord[0];
			}
			// hints will count as an incorrect guess
			++incorrectTries;
			guess = _getch();
		}
		// If we already printed the hint, let's not print the first letter a second time.
		// "\b" backspaces one space.
		if (printedHint)
			std::cout << "\b";
		// print the word and allow the next hint
		std::cout << nextWord << " ";
		printedHint = false;
	}
	// finish up
	std::cout << "\n\n" << incorrectTries << " incorrect guesses. ";
}

void getVerseString(std::string& refToFind, int numVerses, std::string& source, std::string &dest)
{
	// position indicator
	std::string::size_type pos = std::string::npos;

	// find the string
	std::string verseLine;
	pos = source.find(refToFind.c_str(), 0);
	if (pos == std::string::npos) // not in the file
		throw std::invalid_argument{ "Invalid reference." };
	else if (pos != std::string::npos) // Found it!
	{
		for (int i = 0; i < numVerses; ++i)
		{
			while (source[pos] != '\n')
			{
				verseLine += source[pos];
				++pos;
			}
			++pos;
			if (i < numVerses - 1)
				verseLine += "\n+++++\n";
		}
	}
	// got the verse, now return it
	dest = verseLine;
}

void getReference(std::istream& in, std::string &dest)
{
	// ref is the first inputted variable and temp (and possibly tempTwo)
	// contain other parts of the reference (since std::istream gets input
	// separated by spaces) e.g. Input: Psalms 23:1 would make ref = Psalms
	// and temp = 23:1 using std::cin.
	std::string ref, temp;
	in >> ref;
	if (ref[0] == '<')
	{
		dest = ref;
		return;
	}
	in >> temp;
	// if this reference has a total of 3 space-separated parts,  let's get them
	if (ref == "1" || ref == "2" || ref == "3" || (ref[0] == 'I' && ref[1] != 's'))
	{
		// The Bible I use has "1 Peter", "2 John", etc., not "I Peter", "II John",
		// so this will set the book no. to the format used in the Bible in this program.
		if (ref == "I")
			ref = '1';
		else if (ref == "II")
			ref = '2';
		else if (ref == "III")
			ref = '3';
		// now get the last part of the reference
		std::string tempTwo;
		in >> tempTwo;
		// Check to see if this is a one-chaptered, numbered book.
		if ((ref == "2" || ref == "3") && temp == "John" && !(tempTwo.length() < 3))
			tempTwo = "1:" + tempTwo;
		ref += ' ' + temp + ' ' + tempTwo;
	}
	// four part reference! (Song of Solomon)
	else if (ref == "Song" && (temp == "of" || temp == "Of"))
	{
		temp = "of"; // just in case someone enters Song Of Solomon
		std::string tempTwo, tempThree;
		in >> tempTwo >> tempThree;
		ref += ' ' + temp + ' ' + tempTwo + ' ' + tempThree;
	}
	// just a two part reference here
	else
	{
		// Check for one-chapter books and books with or without an 's'.
		if ((ref == "Obadiah" || ref == "Philemon" || ref == "Jude") && !(temp.length() < 3))
			temp = "1:" + temp;
		else if (ref == "Psalm")
			ref = "Psalms";
		else if (ref == "Revelations")
			ref = "Revelation";
		ref += ' ' + temp;
	}
	dest = ref;
}
