#pragma once
#ifndef CONSOLEDRAWER_H_INCLUDED
#define CONSOLEDRAWER_H_INCLUDED
#include <ncurses.h>
#include "idrawer.h"

class CConsoleDrawer : public IDrawer
{

public:
    CConsoleDrawer();
    virtual ~CConsoleDrawer();

public:
    virtual void DrawPixel(int x, int y, int color);
    virtual void DrawText(int x, int y, const char *Text);
    virtual void FlushBuffer();
    virtual int GetWidth();
    virtual int GetHeight();
};

#endif // CONSOLEDRAWER_H_INCLUDED
