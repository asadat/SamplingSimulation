#include "World.h"
#include <stdio.h>

#include <cstdio>

#define MAX(a,b)    (a>b?a:b)
#define WORLD_WIDTH     128
#define WORLD_LENGTH    128
#define WORLD_MAX_HEIGHT    0.1
#define OBS_MAX_LENGHT  20

World* World::instance = NULL;

World::World():Visualizer()
{
    //RegisterGlDrawing();
    bDrawGhost = false;
    inter_cells_n = WORLD_WIDTH*WORLD_LENGTH/200;}

void World::PopulateWorld(int int_cells)
{
    if(int_cells >0)
        inter_cells_n = int_cells;

//    FILE *f = fopen("result","r+");
//    Vector<3> p;
//    Vector<3> orig;
//    bool orgset = false;
//    float x,y,z;
//    int nn=0;
//    while(fscanf(f,"%f %f %f\n",&x,&y,&z) != EOF)
//    {
//        p[0] = x;
//        p[1] = y;
//        p[2] = z;

//        nn++;
//        p[0] *= 2000;
//        p[1] *= 1000;
//        p[2] /=40;
//        if(!orgset)
//        {
//            orgset = true;
//            orig = p;
//        }

//        p= p-orig;
//        printf("===%f\t%f\t%f\n",p[0],p[1],p[2]);

//       mesh.AddVertex(p);
//    }

//    printf("====== %d\n",nn);

//    fclose(f);

    planes.clear();
    bDraw = true;

    srand(time(NULL));
     //srand(63);

    double worldW = WORLD_WIDTH;
    double worldL = WORLD_LENGTH;
    // floor


    int n = inter_cells_n;
    double maxL = OBS_MAX_LENGHT;

    InsertPlane(-worldW/2, worldL/2, 0, worldW/2, -worldL/2, 0, true);

    int ii=0;
    while(ii<n)
    {
        double clusterSize = 20;
        double x,y,z;
        double lx,ly;
        double rnd = RAND(0,1);
        lx = maxL;//RAND(0.2* maxL, 0.5* maxL);
        ly = maxL;//RAND(0.2* maxL, 0.5* maxL);

        if(false && rnd<1 && planes.size() > n/50)
        {
            double xx,yy;
            int idx = RAND(0,1)*(planes.size()-1);
            xx = (planes[idx].p1[0]+planes[idx].p2[0])/2;
            yy = (planes[idx].p1[1]+planes[idx].p2[1])/2;

            x = xx + RAND((-clusterSize/2+lx),(clusterSize/2-lx));
            y = yy + RAND((-clusterSize/2+ly),(clusterSize/2-ly));
            x = floor(x);
            y = floor(y);
        }
        else
        {
            x = RAND((-worldW/2+lx),(worldW/2-lx));
            y = RAND((-worldL/2+ly),(worldL/2-ly));
            x = floor(x);
            y = floor(y);
        }



        z = /*RAND(0.5, 1) **/ WORLD_MAX_HEIGHT;
        if(GetHeight(x,y)<= 0.0001)
        {
            ii++;
            InsertPlane(x-lx + 0.1, y+ly - 0.1, z, x+lx - 0.1, y-ly + 0.1, z);
        }
        else
        {
            //printf("%f, %f\n", x,y);
        }
    }

}

void World::DrawBox(Vector<3, double> p1, Vector<3, double> p2, bool floor)
{

    //TOP
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
//            bool floor = false;
//            if((p2-p1)[0] > 10)
//            {
//                floor = true;
//            }

            //glColor3f(0,1,0);
            if(bDrawGhost)
            {
                if(floor)
                    glColor4f(233.0/255, 227.0/255, 133.0/255,0.5);
                else
                    glColor4f(205.0/255, 92.0/255, 92.0/255,0.5);
                    //glColor4f(135.0/255, 121.0/255, 78.0/255,0.5);
            }
            else
            {
                if(floor)
                    glColor3f(233.0/255, 227.0/255, 133.0/255);
                else
                    glColor3f(205.0/255, 92.0/255, 92.0/255);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glColor3f(1,1,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }

        glBegin(GL_POLYGON);
        glVertex3f(p1[0], p1[1], p1[2]);
        glVertex3f(p2[0], p1[1], p2[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        glVertex3f(p1[0], p2[1], p2[2]);
        glEnd();
    }

    //LEFT
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
            if(bDrawGhost)
            {
                glColor4f(233.0/255, 227.0/255, 133.0/255,0.5);
            }
            else
            {
                glColor3f(233.0/255, 227.0/255, 133.0/255);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glColor3f(1,1,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }

        glBegin(GL_POLYGON);
        glVertex3f(p1[0], p1[1], p1[2]);
        glVertex3f(p1[0], p1[1], 0);
        glVertex3f(p1[0], p2[1], 0);

        //glVertex3f(p1[0], p1[1], p1[2]);
        glVertex3f(p1[0], p2[1], p2[2]);
        //glVertex3f(p1[0], p2[1], 0);
        glEnd();
    }

    //RIGHT = Left+x2-x1
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
            if(bDrawGhost)
            {
                glColor4f(233.0/255, 227.0/255, 133.0/255,0.5);
            }
            else
            {
                glColor3f(233.0/255, 227.0/255, 133.0/255);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glColor3f(1,1,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }

        glBegin(GL_POLYGON);
        glVertex3f(p2[0], p1[1], p1[2]);
        glVertex3f(p2[0], p1[1], 0);
        glVertex3f(p2[0], p2[1], 0);

        //glVertex3f(p2[0], p1[1], p1[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        //glVertex3f(p2[0], p2[1], 0);
        glEnd();
    }

    //NORTH
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
            if(bDrawGhost)
            {
                glColor4f(233.0/255, 227.0/255, 133.0/255,0.5);
            }
            else
            {
                glColor3f(233.0/255, 227.0/255, 133.0/255);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glColor3f(1,1,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }

        glBegin(GL_POLYGON);
        glVertex3f(p1[0], p1[1], p1[2]);
        glVertex3f(p1[0], p1[1], 0);
        glVertex3f(p2[0], p1[1], 0);

        //glVertex3f(p1[0], p1[1], p1[2]);
        glVertex3f(p2[0], p1[1], p2[2]);
        //glVertex3f(p2[0], p1[1], 0);
        glEnd();

    }

    //SOUTH
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
            if(bDrawGhost)
            {
                glColor4f(233.0/255, 227.0/255, 133.0/255,0.5);
            }
            else
            {
                glColor3f(233.0/255, 227.0/255, 133.0/255);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glColor3f(1,1,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }

        glBegin(GL_POLYGON);
        glVertex3f(p1[0], p2[1], p1[2]);
        glVertex3f(p1[0], p2[1], 0);
        glVertex3f(p2[0], p2[1], 0);

        //glVertex3f(p1[0], p2[1], p1[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        //glVertex3f(p2[0], p2[1], 0);
        glEnd();
    }
}

void World::glDraw()
{
    if(!bDraw)
        return;

//    glLineWidth(2);
//    glColor3f(0,0,1);

//    glBegin(GL_LINES);
//    double L = WORLD_WIDTH, l= 1;
//    for(int i = 0; i<= L/l; i++)
//    {
//        double x,y;
//        x = -L/2;
//        y = -L/2 + ((double)i)*l;

//        glVertex3f(x,y,0);
//        glVertex3f(x+L,y,0);

//        glVertex3f(y,x,0);
//        glVertex3f(y,x+L,0);


//    }
//    glEnd();

    for(int i=0 ; i<planes.size(); i++)
    {
        Vector<3, double> p1 = planes[i].p1;
        Vector<3, double> p2 = planes[i].p2;
        DrawBox(p1,p2, planes[i].floor);
    }

    for(int i=0 ; i<vis_planes.size(); i++)
    {
        Vector<3, double> p1 = planes[i].p1;
        Vector<3, double> p2 = planes[i].p2;
        DrawBox(p1,p2, true);
    }
}

double World::GetHeight(double x, double y)
{
    double height = 0;
    for(int i=0; i<planes.size(); i++)
    {
        if(planes[i].inside(x,y))
        {
            height = (height < planes[i].p1[2]) ? planes[i].p1[2] : height;
        }
    }

    return height + RAND(-0.01,0.01);
}

void World::InsertPlane(double x1, double y1, double z1, double x2, double y2, double z2, bool floor)
{
    Plane2D plane;
    plane.floor = floor;
    plane.p1[0] = x1;
    plane.p1[1] = y1;
    plane.p1[2] = z1;

    plane.p2[0] = x2;
    plane.p2[1] = y2;
    plane.p2[2] = z2;

    planes.push_back(plane);
}

void World::InsertVisPlane(double x1, double y1, double z1, double x2, double y2, double z2)
{
    Plane2D plane;
    plane.p1[0] = x1;
    plane.p1[1] = y1;
    plane.p1[2] = z1;

    plane.p2[0] = x2;
    plane.p2[1] = y2;
    plane.p2[2] = z2;

    vis_planes.push_back(plane);
}

void World::ToggleDraw()
{
    if(bDrawGhost && bDraw)
    {
        bDrawGhost = false;
        bDraw = true;
    }
    else if(!bDrawGhost && !bDraw)
    {
        bDrawGhost = true;
        bDraw = true;
    }
    else
    {
        bDraw = false;
        bDrawGhost = false;
    }
}

double World::GetInterestingness(TooN::Vector<2, double> tl, TooN::Vector<2, double> br)
{
    double result = 0;
    result = GetMaxHeightInRect((tl[0]+br[0])/2, (tl[1]+br[1])/2, MAX(fabs(br[0]-tl[0]),fabs(br[1]-tl[1])-0.01));
    return result;
}

double World::GetMaxHeightInRect(double x, double y, double footprint_l)
{
    double mxheight=0;
    for(double i=x-footprint_l/2;i<x+footprint_l/2; i+=0.9 /*BS_MAX_LENGHT*/)
        for(double j=y-footprint_l/2;j<y+footprint_l/2; j+=0.9 /*OBS_MAX_LENGHT*/)
        {
            double h = GetHeight(i,j);
            mxheight = (mxheight < h)?h:mxheight;
        }
    return mxheight;
}

double World::GetMaxHeight()
{
    return WORLD_MAX_HEIGHT;
}

double World::GetLength()
{
    return WORLD_LENGTH;
}

double World::GetWidth()
{
    return WORLD_WIDTH;
}
