#!/bin/bash
# This script builds the Bible Memory program.

# Get some variables.
USERNAME=`whoami`
HAVECDED="no" # This tells us whether we have changed directories to build the program.
COMPILER="g++"
VERBOSE=
BUILDPATH="."
BIBLEPATH="."
SUDO=
OUTNAME="biblememory"

# This function will build the program.
build()
{
    echo "Compiler: $COMPILER"

    # remove existing binary
    if [ -e "$BUILDPATH/$OUTNAME" ]; then
        $SUDO rm "$BUILDPATH/$OUTNAME"
    fi
    
    echo "Building bible-memory.cpp..."
    $SUDO $COMPILER bible-memory.cpp -o "$BUILDPATH/$OUTNAME" -D PATH="\"$BIBLEPATH/bible.txt\"" $VERBOSE
    if [ "$?" != 0 ]; then
        echo "Build failed, please check $COMPILER output."
        exit 1
    fi
    $SUDO chmod +x "$BUILDPATH/$OUTNAME"
}

print_help()
{
    echo "Build script for the Bible Memory program. By default, builds program in"
    echo "current directory."
    echo "Usage:"
    echo "  -i, --install <type>        Specify how to install the program. Type \'user\'"
    echo "                              to build it for your user account only, and"
    echo "                              type \'system\' to install for all users. Defaults"
    echo "                              to current directory. Note: a system build requires"
    echo "                              sudo privileges."
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
                sudo rm "/bin/$OUTNAME"
            fi
            if [ -e "/home/$USERNAME/.local/bin/$OUTNAME" ]; then
                rm "/home/$USERNAME/.local/bin/$OUTNAME"
            fi
            if [ -e "./$OUTNAME" ]; then
                rm "./$OUTNAME"
            fi
            ;;
        *)
            echo "Aborting."
            exit 3
            ;;
    esac
}

update_program()
{
    git clone "https://github.com/LorenDB/bible-memory.git/"
    FILES=`ls bible-memory`
    rm -rf $FILES $VERBOSE
    cp "bible-memory/*.*" .
    rm -rf "bible-memory"
}

# Our main function. This takes care of all operations.
main()
{
    # Parse args.
    while [ "$1" != "" ]; do
        case "$1" in
            -i|--install)
                shift
                INSTALLTYPE="$1"
                case "$INSTALLTYPE" in
                    u|user)
                        BUILDPATH="/home/$USERNAME/.local/bin"
                        BIBLEPATH="/home/$USERNAME/.local/var/biblememory"
                        ;;
                    s|system)
                        BUILDPATH="/bin"
                        BIBLEPATH="/etc/biblememory"
                        SUDO="sudo"
                        ;;
                    *)
                        ;; # leave the install type as current dir and keep going
                esac
                shift
                ;;
            -c|--compiler)
                shift
                case "$1" in
                    g++)
                        COMPILER="$1"
                        ;;
                    clang++)
                        COMPILER="$1"
                        ;;
                    *)
                        echo "You are attempting to use an unsupported compiler ($COMPILER). If this"
                        echo "compiler works, please create a pull request on Github with an updated"
                        echo "compiler list or send an email to <computersemiexpert@outlook.com>. Do"
                        printf "you want to continue? (y or n) "
                        read ANSWER
                        $ANSWER = "y" && COMPILER="$1"
                        $ANSWER = "n" && exit 2
                esac
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

    build

    # Make sure the Bible file is available
    if [ ! -e "$BIBLEPATH/bible.txt" ]; then
        $SUDO cp "bible.txt" "$BIBLEPATH/bible.txt"
    fi
}

# Run the script. $* passes parameters to main.
main $*
