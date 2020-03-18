#!/bin/bash
# build.sh: builds the Bible Memory program and sets it up for use.

# Get some variables.
USERNAME=`whoami`
HAVECDED="no" # This tells us whether we have changed directories to build the program.
COMPILER="g++"
VERBOSE=
BUILDPATH=
BIBLEPATH=
BUILDTYPE=
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
    echo "  -u, --uninstall     Uninstall the Bible Memory program."
    echo
    echo "If the script fails, type \"\$?\" to get the exit code. Then"
    echo "read the script to find where that particular exit code is used."
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
            -u|--uninstall)
                if [ -e "/bin/biblememory" ]; then
                    sudo rm "/bin/biblememory"
                fi
                if [ -e "/etc/biblememory/bible.txt" ]; then
                    sudo rm "/etc/biblememory/bible.txt"
                    sudo rmdir "/etc/biblememory"
                fi
                if [ -e "/home/$USERNAME/.local/bin/biblememory" ]; then
                    rm "/home/$USERNAME/.local/bin/biblememory"
                fi
                if [ -e "/home/$USERNAME/.local/var/biblememory/bible.txt" ]; then
                    rm "/home/$USERNAME/.local/var/biblememory/bible.txt"
                    rmdir "/home/$USERNAME/.local/var/biblememory/"
                fi
                exit 0
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
            BUILDTYPE="user"
            ;;
        s|S)
            BUILDPATH="/bin"
            BIBLEPATH="/etc/biblememory"
            BUILDTYPE="system"
            SUDO="sudo"
            ;;
        *)
            echo "Invalid option specified! Exiting..."
            exit 1
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
