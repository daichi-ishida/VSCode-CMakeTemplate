#include <cstdio>
#ifdef _MSC_VER
   #include <windows.h>
   #include <conio.h>
   
   void sleep(unsigned int s)
   {
       Sleep(1000 * s);
   }
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

    int _kbhit()
    {
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF)
        {
            ungetc(ch, stdin);
            return 1;
        }
        return 0;
    }

    int _getche()
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
    }

#endif



int main()
{
    int x = 0;
    while(1)
    {
        printf("%d\n", x++);
        sleep(10);
        if(_kbhit() && _getche()=='q')
        {
            printf("q pressed\n");
            break;
        }
    }
    return 0;
}
