# bible-memory
BibleMemory program to help you memorize the Bible.

If you ever think my code needs modified in any way, please send me a pull request.

# Installing
## Linux
### Install
1. Clone this repo to your computer.
2. In a terminal, change to the directory you cloned the repo to and run `chmod +x build.sh`.
3. Run `./build.sh`.  
    > By default, build.sh will build the program in the current directory. However, you have the option of installing the program for all users (a system install) or for the current user only (a user install). Pass the option `-i u` to build.sh for a user install and the option `-i s` for a system install. For advanced usage, pass the option `-h` to build.sh.
4. To run the program, type `biblememory` at the command prompt.

### Uninstall
To remove the program, run `./build.sh -r`.

### Update
Run `./build.sh -u`.

### More help
For more info, run `./build.sh -h`.

## Windows
I have not yet perfected a solution for building on Windows. Importing the code into a Visual Studio project should work just fine, however. In the future, I want to make a build.bat file that will build on Windows. If you would like to help me with this batch file, please contact me and I will give you details of what the batch file should do.

## Other platforms
I have not ported this program to other platforms. If you get it working on your platform, please send me a pull request.
