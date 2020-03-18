@echo off

rem cl biblememory.cpp /O2 /favor:blend /Fe biblememory.exe /D PATH="bible.txt"

echo If you know how to write Windows batch files and Linux shell scripts,
echo please read build.sh and make a similar batch file for Windows and send
echo me a pull request on GitHub. The batch file should be able to install or
echo remove the program. The bible.txt file should be copied to the same
echo directory as the program. The script should pass an appropriate path
echo to the compiler. It should also be able to print help, and ideally
echo should include a verbose mode. Functionality to add the program to the
echo system or user PATH would be nice but not required.

rem /MP # sets the # of threads to use
rem /showIncludes for verbose

@echo on