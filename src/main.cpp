#include <stdio.h>
#include "lab3d.h"


int main()
{
    IDrawer *pDrawer = new CConsoleDrawer();

    CLab3D *pLab3D = new CLab3D(pDrawer, 90);

    pLab3D->Init();

    pLab3D->Start();

    //getchar();

    delete pDrawer;

    return 0;
}
