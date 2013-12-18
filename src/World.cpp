#include "World.h"

#define RAND(a,b)    a+(b-a)*((double)(rand()%1000))/1000
#define WORLD_WIDTH     10
#define WORLD_HEIGHT    10

World* World::instance = NULL;

World::World():Visualizer()
{
    //RegisterGlDrawing();

    srand(time(NULL));
    double worldW = WORLD_WIDTH;
    double worldH = WORLD_HEIGHT;
    // floor


    int n=5;
    double maxL = 3;
    double maxH = 1.5;


    InsertPlane(-worldW/2 - maxL/2, worldH/2+maxL/2, 0, worldW/2+maxL/2, -worldH/2-maxL/2, 0);

    for(int i=0; i<n; i++)
    {
        double x,y,z;
        double lx,ly;
        x = RAND(-worldW/2, worldW/2);
        y = RAND(-worldH/2, worldH/2);
        z = RAND(1, maxH);
        lx = RAND(1, maxL);
        ly = RAND(1, maxL);

        InsertPlane(x-lx/2, y+ly/2, z, x+lx/2, y-ly/2, z);
    }

}

void World::DrawBox(Vector<3, double> p1, Vector<3, double> p2)
{

    //TOP
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
            glColor3f(0,1,0);
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
            glColor3f(0,1,0);
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
            glColor3f(0,1,0);
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
            glColor3f(0,1,0);
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
            glColor3f(0,1,0);
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

    for(int i=0 ; i<planes.size(); i++)
    {
        Vector<3, double> p1 = planes[i].p1;
        Vector<3, double> p2 = planes[i].p2;
        DrawBox(p1,p2);
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

    return height;
}

void World::InsertPlane(double x1, double y1, double z1, double x2, double y2, double z2)
{
    Plane2D plane;
    plane.p1[0] = x1;
    plane.p1[1] = y1;
    plane.p1[2] = z1;

    plane.p2[0] = x2;
    plane.p2[1] = y2;
    plane.p2[2] = z2;

    planes.push_back(plane);
}
