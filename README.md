# TotalReqall
A program for memorizing the Bible and other works. Written in C++ with Qt, KDE Frameworks, and the Sword library.

Features:
- Memorize one Bible verse or select multiple verses from the same chapter and book.
- Create your own content to memorize.
- Save items to memorize later.  
 > Every time you select a verse or enter custom content and click "Memorize", that item will be saved automatically.  
- Split large items into smaller chunks for easier memorization.  
 > Note that currently TotalReqall will only split content at line breaks. If double-spaced breaks exist, TotalReqall will split the content at the double-spaced break. Otherwise, the content will be split at single-spaced line breaks. There is no way to set what size of content should be split or how large of chunks it should be split into. The splitting feature can be disabled if so desired. Stay tuned for more development in this area.  
- Listen to items using text-to-speech.  
 > This will only work on systems with text-to-speech available. See [the Qt docs](https://doc.qt.io/qt-5/qtspeech-index.html) for more details.  

## Installing
You will need to install the Sword library. Also, make sure that you have at least one Sword module installed (failure to provide a module may cause bad behavior).

If you want text-to-speech, you will need to install the Qt Speech module as well. If you don't, pass `-DTTS_ENABLED=OFF` to cmake.

### Debian/Ubuntu
```bash
# sword-text-kjv is the King James Version module for Sword
# If you don't want text-to-speech, you don't need libqt5texttospeech5-dev
sudo apt install libqt5texttospeech5-dev sword-devel sword-text-kjv
```

Build and install with cmake.

```bash
mkdir build && cd build
cmake .. # don't forget to pass -DTTS_ENABLED=OFF if desired
cmake --build .
sudo cmake --install .
```

## Contributing
All MRs will be reviewed, even if they are not accepted. Please run `clang-format -i *.cpp *.h` in the root of the repo before committing.

## Bugs
Use the TotalReqall product on [bugs.kde.org](https://bugs.kde.org).

## Contact
Drop a line at [#totalreqall:kde.org](https://matrix.to/#/#totalreqall:kde.org) or join #kde-totalreqall on Freenode. Note: Matrix is preferred as the IRC bridge may not work (and because it's newer and better).

## What's with the name?
This program is designed to help you memorize things, hence "Total Recall". The *q* in "TotalReqall" references the fact that this software is made using Qt.