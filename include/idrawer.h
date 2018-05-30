#pragma once
#ifndef IDRAWER_H_INCLUDED
#define IDRAWER_H_INCLUDED

class IDrawer
{
public:
    virtual ~IDrawer() {};
    virtual void DrawPixel(int x, int y, int color) = 0;
    virtual void DrawText(int x, int y, const char *Text) = 0;
    virtual void FlushBuffer() = 0;
    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
};

#endif // IDRAWER_H_INCLUDED
