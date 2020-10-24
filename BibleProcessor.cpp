// BibleProcessor.cpp: contains logic for extracting verses from the Bible bundled with this program.
// Copyright Loren Burkholder 2020.

#include <string>
#include <exception>

#include <QString>
#include <QFile>

void getVerseString(std::string& refToFind, int numVerses, std::string& source, std::string &dest, std::string* oldRef)
{
    // position indicator
    std::string::size_type pos = std::string::npos;
    std::string verseLine;
    bool shouldParseOffset = false;
    bool reverse = false;
    int offset = 0;

    // parse any options
    if (refToFind[0] == '<')
    {
        if (refToFind[1] == '+')
        {
            std::string numOffset = refToFind;
            numOffset.erase(0, 2);
            offset = std::stoi(numOffset);
            shouldParseOffset = true;
        }
        else if (refToFind[1] == '-')
        {
            std::string numOffset = refToFind;
            numOffset.erase(0, 2);
            offset = std::stoi(numOffset) + 1; // need to back up past the one we just did
            reverse = true;
            shouldParseOffset = true;
        }
        refToFind = *oldRef;
    }
    // find the string
    pos = source.find(refToFind.c_str(), 0);
    if (pos == std::string::npos) // not in the file
        throw std::invalid_argument{ "Invalid reference." };
    else // Found it!
    {
        if (shouldParseOffset)
        {
            for (int i = 0; i < offset; ++i)
            {
                if (reverse)
                {
                    --pos; // move past the previous '\n' found
                    pos = source.rfind("\n", pos);
                }
                else
                    pos = source.find("\n", pos);
            }
            ++pos; // move past the '\n' to avoid strange behavior
        }
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
