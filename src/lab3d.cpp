#define _USE_MATH_DEFINES


#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>
#include "lab3d.h"
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <list>
#include <iterator>

CLab3D::CLab3D(IDrawer *pDrawer, int fov)
{
    _height = pDrawer->GetHeight();
    _width = pDrawer->GetWidth();
    _fov = fov;
    m_pDrawer = pDrawer;
}
struct SPlayer
{
    Geom::SPoint pos;
    int angle;
};

#define D2R(X) ( (X) * M_PI / 180.0 )

enum Keys
{
    NoKey,
    Up,
    Down,
    Left,
    Right,
};

Keys getKey()
{
    timeout(0);
    int c = getch();
    switch (c)
    {
        case 27 :
            c = getch();
            switch (c)
            {
                case 91 :
                    c = getch();
                    switch (c)
                    {
                        case 65 :
                            return Keys::Up;
                        case 66 :
                            return Keys::Down;
                        case 67 :
                            return Keys::Right;
                        case 68 :
                            return Keys::Left;
                    }
                    break;
            }
            break;
    }
    return Keys::NoKey;
}

void CLab3D::Start()
{
    const double STEP_DIST = 2.0;
    SPlayer player;
    player.pos.x = 502;
    player.pos.y = 0;
    player.angle = 0;

    double maxDistance = 1000.0;
    initscr();


/*
    std::vector<std::chrono::nanoseconds> diffTimeVec;

    std::list<std::chrono::nanoseconds> TimeList;
    std::list<std::chrono::nanoseconds>::iterator itSecond, itFirst;
    std::list<std::chrono::nanoseconds>::reverse_iterator itReverse;

    std::chrono::nanoseconds medianValue{};
*/

    std::list<std::chrono::duration<float>> TimeList;
    std::list<std::chrono::duration<float>>::iterator itSecond, itFirst;

    std::chrono::duration<float> medianValue;

    double maxSizelist = 300.0;

    while(1)
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        switch (getKey())
        {
            case Keys::Up:
                player.pos.x += cos(D2R(player.angle)) * STEP_DIST;
                player.pos.y += sin(D2R(player.angle)) * STEP_DIST;
                break;
            case Keys::Down:
                player.pos.x -= cos(D2R(player.angle)) * STEP_DIST;
                player.pos.y -= sin(D2R(player.angle)) * STEP_DIST;
                break;
            case Keys::Right:
                player.angle -= 1;
                break;
            case Keys::Left:
                player.angle += 1;
                break;
        }



    /*
        unsigned char data[3];
        int bytes = read(fd, data, sizeof(data));
        if(bytes > 0)
        {
                int left = data[0] & 0x1;
                int right = data[0] & 0x2;
                int middle = data[0] & 0x4;

                char x = data[1];
                player.angle += x;

                char y = data[2];
                printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
                player.pos.y += y;
        }
*/


        double startA = (double)player.angle + (double)_fov / 2.0;

        double deltaA = (double)_fov / 2.0;

        Geom::SPoint sP;
        sP.x = cos(D2R(startA)) * ( 1.0 / cos(D2R(deltaA)) ) * maxDistance + player.pos.x;
        sP.y = sin(D2R(startA)) * ( 1.0 / cos(D2R(deltaA)) ) * maxDistance + player.pos.y;
        Geom::SPoint zP;
        zP.x = cos(D2R(player.angle)) * ( maxDistance ) + player.pos.x;
        zP.y = sin(D2R(player.angle)) * ( maxDistance ) + player.pos.y;

        Geom::SPoint dP;
        dP.x = ( sP.x - zP.x ) / (double) (_width / 2.0);
        dP.y = ( sP.y - zP.y ) / (double) (_width / 2.0);

        //double dA = (double)_fov / (double)_width; //maybe width?

        std::vector <double> DistVec;

        for(int i = 0; i < _width; i++) //maybe width?
        {
            Geom::SSegment seg;
            seg.ptBegin = player.pos;

            Geom::SPoint dPos = (dP * (float)i);
            seg.ptEnd = sP - dPos ;
            /*
            double cA = startA + dA * i;
            double cARad = (cA * M_PI) / 180.0;
            seg.ptEnd.x =  cos(cARad) * maxDistance + player.pos.x;
            seg.ptEnd.y =  sin(cARad) * maxDistance + player.pos.y;

            */
            double distToIntersect = maxDistance / cos(D2R(deltaA));
            //printf(" map %d ", _map.size());

            for(int iMapSeg = 0; iMapSeg < _map.size(); iMapSeg++)
            {
                Geom::SPoint ptInter;
                bool inters = FindIntersect(&seg, &_map[iMapSeg], ptInter);
                //printf(" seg %d,%d ", seg.ptEnd.x, seg.ptEnd.y);
                if(inters)
                {
                //printf(" int %d,%d ", ptInter.x, ptInter.y);
                    double tempDist = FindDistance(seg.ptBegin, ptInter);
                    //printf(" tmpdist %f ", tempDist);
                    if(tempDist < distToIntersect)
                        distToIntersect = tempDist;
                }
            }
            double sD = seg.Abs();
            double zD = Geom::FindDistance( zP, player.pos );
            double dD = Geom::FindDistance( seg.ptEnd, player.pos );
            double zdD = Geom::FindDistance( zP, seg.ptEnd );
            double relAngle = (((dD * dD) + (zD * zD) - (zdD * zdD))/(2 * zD * dD));
            double AngleCoeff = (zD / sD) * relAngle;
            DistVec.push_back(distToIntersect * relAngle);
        }

        double DistCoeff = (double)_height * 1.5 / maxDistance;


        for(int i = 0; i < DistVec.size(); i++ )
        {


            //printf("%f ", DistVec[i]);

           int wallHeight = (maxDistance - DistVec[i]) * DistCoeff;
            //printf("%d ", wallHeight);
            //Drawing in center.
            double pointX = ((double)_height + (double)wallHeight) / 2.0 - 1.25;
             pointX = round(pointX);
            //pointX = (int)pointX + 0.5;



            //printf("%f ", pointX);
            for(int j = 0; j < wallHeight; j++ )
            {
                 m_pDrawer->DrawPixel(i, pointX, '#');
                 pointX--;
                 //printf("%d ", pointX);
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> diffTime = endTime - startTime;

        if(TimeList.size() == 0)
        {
            TimeList.push_back(diffTime);
        }
        else if (TimeList.size() >= maxSizelist)
        {
            itFirst = itSecond = TimeList.begin();

            advance(itSecond, maxSizelist / 3 );
            //TODO IT is Right?
            //TimeList.erase(TimeList.begin(), itSecond);
            TimeList.erase(itFirst, itSecond);

            itSecond = TimeList.end();
            advance(itSecond, -(maxSizelist / 3));
            TimeList.erase(itSecond, TimeList.end());
            /*itReverse = TimeList.rbegin();
            advance(itReverse, maxSizelist / 3);
            itSecond = TimeList.end();
            TimeList.erase(itReverse, itSecond);*/
        }
        else
        {
            itSecond = --TimeList.end();
            if(diffTime > *itSecond)
            {
                TimeList.push_back(diffTime);
            }
            else
            {
                for(itFirst = TimeList.begin(); itFirst != TimeList.end(); itFirst++)
                {
                    if(diffTime > *itFirst)
                    {
                        continue;
                    }
                    TimeList.insert(itFirst, diffTime);
                    break;
                }
            }
        }

        itFirst = TimeList.begin();

        if(!(TimeList.size() % 2))
        {
            advance(itFirst, (TimeList.size() / 2));
            auto tempMed1 = *itFirst--;
            auto tempMed2 = *itFirst;
            medianValue = (tempMed1 + tempMed2) / 2;
        }
        if((TimeList.size() % 2))
        {
            advance(itFirst, (TimeList.size() / 2));
            medianValue = *itFirst;
        }


     //   std::chrono::seconds sec{1};
      //  std::chrono::duration_cast<std::chrono::duration<float>>(sec);
//        medianValue = sec / medianValue;

        char str[255];
        sprintf(str, "X:%d Y:%d A:%d FPS:%d", player.pos.x, player.pos.y, player.angle,  std::chrono::duration_cast<std::chrono::nanoseconds>(medianValue).count() );
        m_pDrawer->DrawText(3, 0, str);
        m_pDrawer->FlushBuffer();
        usleep(1000 * 10);


    }
}

bool Geom::FindIntersect(const SSegment *seg, const SSegment *_map, SPoint &ptInter)
{
    SPoint start1 = seg->ptBegin;
    SPoint finish1 = seg->ptEnd;

    SPoint start2 = _map->ptBegin;
    SPoint finish2 = _map->ptEnd;
    SPoint dir1;
    SPoint dir2;
    dir1.x = finish1.x - start1.x;
    dir1.y = finish1.y - start1.y;

    dir2.x = finish2.x - start2.x;
    dir2.y = finish2.y - start2.y;

    //считаем уравнения прямых проходящих через отрезки
    float a1 = -dir1.y;
    float b1 = +dir1.x;
    float d1 = -(a1*seg->ptBegin.x + b1*seg->ptBegin.y);

    float a2 = -dir2.y;
    float b2 = +dir2.x;
    float d2 = -(a2*_map->ptBegin.x + b2*_map->ptBegin.y);

    //подставляем концы отрезков, для выяснения в каких полуплоскотях они
    float seg1_line2_start = a2*seg->ptBegin.x + b2*seg->ptBegin.y + d2;
    float seg1_line2_end = a2*seg->ptEnd.x + b2*seg->ptEnd.y + d2;

    float seg2_line1_start = a1*_map->ptBegin.x + b1*_map->ptBegin.y + d1;
    float seg2_line1_end = a1*_map->ptEnd.x + b1*_map->ptEnd.y + d1;

    if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0)
        return false;

    float u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);

    ptInter =  start1 + dir1 * u;

    return true;
}

namespace Geom
{
    double FindDistance(const SPoint &pt1, const SPoint &pt2)
    {
        double dX = pt1.x - pt2.x;
        double dY = pt1.y - pt2.y;
        return sqrt(dX * dX + dY * dY);
    }
}
void CLab3D::Init()
{
    _map = LoadMap("../data/map.txt");

}

SegmentsVec CLab3D::LoadMap(const char * pFilePath)
{
    SegmentsVec segments;

    std::ifstream ifs;
    ifs.open(pFilePath);

    if(ifs.is_open())
    {
        std::cout << "File Open";

        for (int i = 0; !ifs.eof(); i++)
        {
            Geom::SSegment segment;

            ifs >> segment.ptBegin.x;
            ifs >> segment.ptBegin.y;
            ifs >> segment.ptEnd.x;
            ifs >> segment.ptEnd.y;
            segments.push_back(segment);
        }
        ifs.close();
    }
    else
    {
        std::cout << "Can't open file";
    }
    return segments;
}


