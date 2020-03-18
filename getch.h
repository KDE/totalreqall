#if defined (__linux__)
    #include <termios.h>
    #include <unistd.h>
    #include <cstdio>
    // The following two functions came from zobayer.
    // See <http://zobayer.blogspot.com/2010/12/getch-getche-in-gccg.html>.
    int _getch()
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
    int _getche()
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#elif defined (_WIN32)
    #include <conio.h>
#else
    #error "Please port the _getch() and _getche() functions to your platform. Try the Linux code, perhaps?"
#endif
