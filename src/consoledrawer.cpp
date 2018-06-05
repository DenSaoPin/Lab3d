#include "ConsoleDrawer.h"
#include <ncurses.h>
#include <unistd.h>
void CConsoleDrawer::DrawPixel(int x, int y, int color)
{

    char symbol = (char)color;
    //start_color();
    mvprintw(y, x, "#");
    return;
   // refresh();
}
CConsoleDrawer::CConsoleDrawer()
{
    initscr();
    noecho();
    curs_set(FALSE);
}
CConsoleDrawer::~CConsoleDrawer()
{
    endwin();
}

void CConsoleDrawer::FlushBuffer()
{
    refresh();
    //clear();
    erase();

    //usleep(100);
    //clearok(stdscr, false);
}

 int CConsoleDrawer::GetWidth()
{
    int max_y = 0;
    int max_x = 0;
    getmaxyx(stdscr, max_y, max_x);
    return max_x;
}

int CConsoleDrawer::GetHeight()
{
    int max_y = 0;
    int max_x = 0;
    getmaxyx(stdscr, max_y, max_x);
    return max_y;
}
void CConsoleDrawer::DrawText(int x, int y, const char *Text)
{
    mvprintw(y, x, Text);
}
