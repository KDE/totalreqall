# TotalReqall
A program for memorizing the Bible and other works. Written in C++ with Qt, KDE Frameworks, and the Sword library.

## Usage
Start the program and choose a book, chapter, and verse. If you want to familiarize yourself with the verse, click "Display verse". Otherwise, click "Memorize verse" to memorize it.

## Installing
You will need to install the Sword and cmark libraries, including the development files. Also, make sure that you have the KJV Sword module installed (failure to provide the KJV module will cause a segfault on Linux). Then build with cmake.

## Contributing
All MRs will be reviewed, even if they are not accepted. Please run `clang-format -i *.cpp *.h` in the root of the repo before committing.

## Bugs
Use the TotalReqall product on [bugs.kde.org](https://bugs.kde.org).

## Contact
Drop a line at [#totalreqall:kde.org](https://matrix.to/#/#totalreqall:kde.org) or join #kde-totalreqall on Freenode. Note: Matrix is preferred as the IRC bridge may not work (and because it's newer and better).

## What's with the name?
This program is designed to help you memorize things, hence "Total Recall". The *q* in "TotalReqall" references the fact that this software is made using Qt.