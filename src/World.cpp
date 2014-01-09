#include "World.h"

#define WORLD_WIDTH     100
#define WORLD_LENGTH    100
#define WORLD_MAX_HEIGHT    10

World* World::instance = NULL;

World::World():Visualizer()
{
    //RegisterGlDrawing();

    bDraw = true;

    srand(time(NULL));

    double worldW = WORLD_WIDTH;
    double worldL = WORLD_LENGTH;
    // floor


    int n = WORLD_WIDTH*WORLD_LENGTH/2000;
    double maxL = 30;

    InsertPlane(-worldW/2, worldL/2, 0, worldW/2, -worldL/2, 0);

    for(int i=0; i<n; i++)
    {
        double x,y,z;
        double lx,ly;
        lx = RAND(1, maxL/2);
        ly = RAND(1, maxL/2);
        x = RAND((-worldW/2+lx),(worldW/2-lx));
        y = RAND((-worldL/2+ly),(worldL/2-ly));
        z = RAND(1, WORLD_MAX_HEIGHT);

        InsertPlane(x-lx, y+ly, z, x+lx, y-ly, z);
    }

}

void World::DrawBox(Vector<3, double> p1, Vector<3, double> p2)
{

    //TOP
    for(int i=0; i<2; i++)
    {
        if(i==0)
        {
            //glColor3f(0,1,0);
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

    glLineWidth(2);
    glColor3f(0,0,1);

    glBegin(GL_LINES);
    double L = WORLD_WIDTH, l= WORLD_WIDTH/20;
    for(int i = 0; i<= L/l; i++)
    {
        double x,y;
        x = -L/2;
        y = -L/2 + ((double)i)*l;

        glVertex3f(x,y,0);
        glVertex3f(x+L,y,0);

        glVertex3f(y,x,0);
        glVertex3f(y,x+L,0);


    }
    glEnd();

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

    return height + RAND(-0.01,0.01);
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

double World::GetMaxHeightInRect(double x, double y, double footprint_l)
{
    double mxheight=0;
    for(double i=x-footprint_l/2;i<x+footprint_l/2; i+=0.2)
        for(double j=y-footprint_l/2;j<y+footprint_l/2; j+=0.2)
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
