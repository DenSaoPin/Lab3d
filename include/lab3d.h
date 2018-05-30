#pragma once
#include<vector>
#include "ConsoleDrawer.h"

namespace Geom
{
    struct SPoint
    {
        int x;
        int y;

        SPoint operator*(const float &d) const
        {
            SPoint result;
            result.x = x * d;
            result.y = y * d;
            return result;
        }
        SPoint operator+(const SPoint &other)
        {
            SPoint result;
            result.x = x + other.x;
            result.y = y + other.y;
            return result;
        }
        SPoint operator-(const SPoint &other)
        {
            SPoint result;
            result.x = x - other.x;
            result.y = y - other.y;
            return result;
        }
    };

    double FindDistance(const SPoint &pt1, const SPoint &pt2);

    struct SSegment
    {
        SPoint ptBegin;
        SPoint ptEnd;

        double Abs()
        {
            return FindDistance(ptBegin, ptEnd);
        }
    };

    bool FindIntersect(const SSegment *seg, const SSegment *_map, SPoint &ptInter);
};
typedef std::vector<Geom::SSegment> SegmentsVec;


class CLab3D
{
    int _height;
    int _width;
    int _fov;
    SegmentsVec _map;
    IDrawer * m_pDrawer;
public:

    CLab3D(IDrawer *, int fov);

    void Start();

    void Init();



private:

    SegmentsVec LoadMap(const char * pFilePath);

};
