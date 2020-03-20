#!/bin/bash
# build.sh: builds the Bible Memory program and sets it up for use.

# Get some variables.
USERNAME=`whoami`
HAVECDED="no" # This tells us whether we have changed directories to build the program.
COMPILER="g++"
VERBOSE=
BUILDPATH=
BIBLEPATH=
SUDO=

# This function will build the program.
build()
{
    echo "Compiler: $COMPILER"
    if [ -e "$BUILDPATH/biblememory" ]; then
        $SUDO rm "$BUILDPATH/biblememory"
    fi
    echo "Building BibleMemory.cpp..."
    $SUDO $COMPILER BibleMemory.cpp -D "PATH=\"$BIBLEPATH/bible.txt\"" -o "$BUILDPATH/biblememory" $VERBOSE
    if [ "$?" != 0 ]; then
        echo "Build error!"
        exit 1
    fi
    $SUDO chmod +x "$BUILDPATH/biblememory"
}

print_help()
{
    echo "Build script for the Bible Memory program."
    echo "Usage:"
    echo "  -c, --compiler      Specify the compiler to use. The script"
    echo "                      currently supports g++ and clang++."
    echo "  -v, --verbose       Show detailed information from the compiler"
    echo "                      about the build."
    echo "  -h, --help          Show this message."
    echo "  -r, --remove        Remove the Bible Memory program."
    echo
    echo "If the script fails, type \"\$?\" to get the exit code. Then"
    echo "read the script to find where that particular exit code is used."
}

uninstall_program()
{
    printf "Are you sure you want to remove the program? "
    read SHOULDPROCEED
    case "$SHOULDPROCEED" in
        "y")
            if [ -e "/bin/biblememory" ]; then
                sudo rm "/bin/biblememory"
            fi
            if [ -e "/etc/biblememory/bible.txt" ]; then
                sudo rm -r "/etc/biblememory"
            fi
            if [ -e "/home/$USERNAME/.local/bin/biblememory" ]; then
                rm "/home/$USERNAME/.local/bin/biblememory"
            fi
            if [ -e "/home/$USERNAME/.local/var/biblememory/bible.txt" ]; then
                rm -r "/home/$USERNAME/.local/var/biblememory"
            fi
            ;;
        *)
            echo "Aborting."
            exit 0
            ;;
    esac
}

update_program()
{
    git clone "https://github.com/LorenDB/bible-memory.git/"
    FILES=`ls bible-memory`
    rm $FILES -f -r $VERBOSE
    cp bible-memory/*.* .
    rm -r bible-memory
}

# Our main function. This takes care of all operations.
main()
{
    # Parse args.
    while [ "$1" != "" ]; do
        case "$1" in
            -c|--compiler)
                shift
                COMPILER="$1"
                shift
                ;;
            -v|--verbose)
                VERBOSE="-v"
                shift
                ;;
            -h|--help)
                print_help
                exit 0
                ;;
            -r|--remove)
                uninstall_program
                exit 0
                ;;
            -u|--update)
                update_program
                shift
                ;;
            *)
                shift
                ;;
        esac
    done

    # Do we build for the user or system-wide?
    printf "Do you want to build for this (u)ser or (s)ystemwide? "
    read ANSWER
    case "$ANSWER" in
        u|U)
            BUILDPATH="/home/$USERNAME/.local/bin"
            BIBLEPATH="/home/$USERNAME/.local/var/biblememory"
            ;;
        s|S)
            BUILDPATH="/bin"
            BIBLEPATH="/etc/biblememory"
            SUDO="sudo"
            ;;
        *)
            echo "Invalid option specified! Exiting..."
            exit 2
    esac

    # Build.
    build
    
    # Make sure that the Bible file is available.
    if [ ! -e "$BIBLEPATH" ]; then
        $SUDO mkdir "$BIBLEPATH"
    fi
    if [ ! -e "$BIBLEPATH/bible.txt" ]; then
        echo "Copying Bible data file..."
        $SUDO cp bible.txt "$BIBLEPATH/bible.txt"
    fi
}

# Run the script. $* passes parameters to main.
main $*
