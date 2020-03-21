#!/bin/bash
# This script builds the Bible Memory program.

# Get some variables.
USERNAME=`whoami`
HAVECDED="no" # This tells us whether we have changed directories to build the program.
COMPILER="g++"
VERBOSE=
BUILDPATH=
BIBLEPATH=
SUDO=
OUTNAME="biblememory"

# This function will build the program.
build()
{
    echo "Compiler: $COMPILER"
    if [ -e "$BUILDPATH/$OUTNAME" ]; then
        $SUDO rm "$BUILDPATH/$OUTNAME"
    fi
    echo "Building bible-memory.cpp..."
    $SUDO $COMPILER bible-memory.cpp -o "$BUILDPATH/$OUTNAME" $VERBOSE
    if [ "$?" != 0 ]; then
        echo "Build error!"
        exit 1
    fi
    $SUDO chmod +x "$BUILDPATH/$OUTNAME"
}

print_help()
{
    echo "Build script for the Bible Memory program."
    echo "Usage:"
    echo "  -b, --buildtype <type>      Specify how to build the program. Type \'user\'"
    echo "                              to build it for your user account only, and"
    echo "                              type \'system\' to install for all users. Note:"
    echo "                              a system build requires sudo priveliges."
    echo "  -c, --compiler <compiler>   Specify the compiler to use. The script"
    echo "                              currently supports g++ and clang++."
    echo "  -h, --help                  Show this message."
    echo "  -o, --out <name>            Specify the name of the newly compiled program."
    echo "  -r, --remove                Remove the Bible Memory program. If you have"
    echo "                              compiled the program with a non-default command"
    echo "                              name, you should specify the -o <name> option"
    echo "                              before the -r option."
    echo "  -u, --update                Update the Bible Memory program."
    echo "  -v, --verbose               Show detailed information from the compiler"
    echo "                              about the build."
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
            if [ -e "/bin/$OUTNAME" ]; then
                sudo rm "/bin/ttt"
            fi
            if [ -e "/home/$USERNAME/.local/bin/$OUTNAME" ]; then
                rm "/home/$USERNAME/.local/bin/$OUTNAME"
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
    cp "bible-memory/*.*" .
    rm -r "bible-memory"
}

# Our main function. This takes care of all operations.
main()
{
    # Parse args.
    while [ "$1" != "" ]; do
        case "$1" in
            -b|--buildtype)
                shift
                BUILDTYPE="$1"
                case "$BUILDTYPE" in
                    user)
                        BUILDPATH="/home/$USERNAME/.local/bin"
                        ;;
                    system)
                        BUILDPATH="/bin"
                        SUDO="sudo"
                        ;;
                    *)
                        echo "Invalid build type! Exiting..."
                        exit 2
                        ;;
                esac
                shift
                ;;
            -c|--compiler)
                shift
                COMPILER="$1"
                shift
                ;;
            -h|--help)
                print_help
                exit 0
                ;;
            -o|--out)
                shift
                OUTNAME="$1"
                shift
                ;;
            -r|--remove)
                uninstall_program
                exit 0
                ;;
            -u|--update)
                update_program
                shift
                ;;
            -v|--verbose)
                VERBOSE="-v"
                shift
                ;;
            *)
                shift
                ;;
        esac
    done

    # Build.
    build
}

# Run the script. $* passes parameters to main.
main $*
