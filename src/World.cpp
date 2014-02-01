#include "World.h"
#include <stdio.h>
#include "cvd/image_io.h"
#include "cvd/image.h"
#include <cstdio>
#include <stdio.h>

#define MAX(a,b)    (a>b?a:b)
#define WORLD_WIDTH     128.0
#define WORLD_LENGTH    128.0
#define WORLD_MAX_HEIGHT    0.1
#define OBS_MAX_LENGHT  30

using namespace CVD;
World* World::instance = NULL;
unsigned int World::textureName = 0;

World::World():Visualizer()
{

    //textureName = 0;
    //RegisterGlDrawing();
    bDrawGhost = false;
    inter_perc = 20;
    inter_cells_n = WORLD_WIDTH*WORLD_LENGTH/200;
}

void World::PopulateWorldFromImage(char * imagepath)
{
    planes.clear();
    CVD::Image<CVD::Rgb<CVD::byte> > img;
    CVD::img_load(img,imagepath);
    CVD::ImageRef size = img.size();
    bDraw = true;
    double cellSize =WORLD_WIDTH/size.x;
    int n=0;
    //printf("Size: %d %d\n", size.x, size.y);
    for(int i=0; i<size.x; i++)
        for(int j=0; j<size.y; j++)
        {
            //printf("at: %d %d\n", i, j);

            CVD::Rgb<CVD::byte> c = img[j][i];
            //if(c.red < 100 && c.blue < 100 && c.green > 100)
            if(c.red > 250 && c.green > 250 && c.blue <5 ||
                    c.red < 5 && c.green > 250 && c.blue <5 ||
                    c.red > 250 && c.green > 250 && c.blue >250)//(c.red < 150 && c.blue < 100 && c.green > 100) || (c.red > 200 && c.blue < 150 && c.green > 200))
            {
                n++;
                double x1,x2,y1,y2;
                x1 = -cellSize*size.x/2 + cellSize*i;
                x2 = x1+cellSize;
                y1 = cellSize*size.y/2 - cellSize*j;
                y2 = y1-cellSize;
                InsertPlane(x1,y1,WORLD_MAX_HEIGHT,x2,y2,WORLD_MAX_HEIGHT);
                //printf("%f %f %f %f\n", x1, y1, x2, y2);
            }
        }


    InsertPlane(-WORLD_WIDTH/2, WORLD_LENGTH/2, 0, WORLD_WIDTH/2, -WORLD_LENGTH/2, 0, true);

    //char tex[100];
    std::string s;
    s.append(imagepath);
    CVD::img_load(texture_img, "tex"+s);
    //printf("finnished loading... %d cells\n", n);

}

void World::PopulateWorld(int inte_per, int int_cells)
{
    //PopulateWorldFromImage();
    inter_perc = inte_per;
    if(int_cells >0)
        inter_cells_n = int_cells;

    planes.clear();
    bDraw = true;

    srand(time(NULL));
     //srand(63);

    double worldW = WORLD_WIDTH;
    double worldL = WORLD_LENGTH;
    // floor


    int n = inter_cells_n;
    //double maxL = OBS_MAX_LENGHT;
    double maxL = sqrt((((double)inter_perc)/100.0)*WORLD_WIDTH*WORLD_LENGTH/int_cells);

    //printf("====== %f\n", maxL);

    int trials = 0;
    //int ii=0;
    while(planes.size() < n)
    {
        trials++;
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
            x = RAND((-worldW/2+lx/2),(worldW/2-lx/2));
            y = RAND((-worldL/2+ly/2),(worldL/2-ly/2));
            x = floor(x);
            y = floor(y);
        }

        z = WORLD_MAX_HEIGHT;
        if(GetMaxHeightInRect(x,y,maxL)<= 0.05)
        {
            //ii++;
            InsertPlane(x-lx/2 + 0.1, y+ly/2 - 0.1, z, x+lx/2 - 0.1, y-ly/2 + 0.1, z);
        }
        else
        {
            if(trials > 500)
            {
                trials=0;
                planes.clear();
            }
            //printf("%f, %f\n", x,y);
        }
    }

    InsertPlane(-worldW/2, worldL/2, 0, worldW/2, -worldL/2, 0, true);


}

void World::DrawBox(Vector<3, double> p1, Vector<3, double> p2, bool floor)
{

    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);
    //TOP
    for(int i=0; i<1; i++)
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
                   // glColor3f(205.0/255, 92.0/255, 92.0/255);
                     glColor3f(95.0/255, 252.0/255, 92.0/255);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            if(floor/* && textureName != 0*/)
            {
                //printf("hi\n");
//                glColor3f(1,1,1);
//                glEnable(GL_TEXTURE_2D);
//                glBindTexture(GL_TEXTURE_2D, textureName);
            }

        }
        else
        {
            glColor3f(1,1,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }


        if(floor && texture_img.size().x > 0)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureName);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin (GL_QUADS);
            glTexCoord2f (0.0, 0.0);
            glVertex3f(p1[0], p1[1], p1[2]);
            glTexCoord2f (1.0, 0.0);
            glVertex3f(p2[0], p1[1], p2[2]);
            glTexCoord2f (1.0, 1.0);
            glVertex3f(p2[0], p2[1], p2[2]);
            glTexCoord2f (0.0, 1.0);
            glVertex3f(p1[0], p2[1], p2[2]);
            glEnd ();

//            glTexCoord2f(0,0);
//            glVertex3f(p1[0], p1[1], p1[2]);
//            glTexCoord2f(0,1);
//            glVertex3f(p2[0], p1[1], p2[2]);
//            glTexCoord2f(1,1);
//            glVertex3f(p2[0], p2[1], p2[2]);
//            glTexCoord2f(1,0);
//            glVertex3f(p1[0], p2[1], p2[2]);
        }
        else
        {
            glBegin(GL_POLYGON);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p1[1], p2[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p1[0], p2[1], p2[2]);
            glEnd();
        }

    }

    return ;
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

    if(textureName == 0 && texture_img.size().x > 0)
    {
        //CVD::Image<CVD::Rgb<CVD::byte> > imgtex;
        //CVD::img_load(imgtex,"texture.jpg");

        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &World::textureName);
        glBindTexture(GL_TEXTURE_2D, World::textureName);
        glTexImage2D(GL_TEXTURE_2D,
            0, GL_RGB,
            texture_img.size().x, texture_img.size().y,
            0, GL_RGB,
            GL_UNSIGNED_BYTE,
            texture_img.data());
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
       // glDisable(GL_TEXTURE_2D);

         printf("hi %u %d %d\n", World::textureName, texture_img.size().x, texture_img.size().y);

    }
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

//    glColor3f(1.0f, 1.0f, 1.0f);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, textureName);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    glBegin (GL_QUADS);
//    glTexCoord2f (0.0, 0.0);
//    glVertex3f (0.0, 0.0, 10);
//    glTexCoord2f (1.0, 0.0);
//    glVertex3f (10.0, 0.0, 10.0);
//    glTexCoord2f (1.0, 1.0);
//    glVertex3f (10.0, 10.0, 10.0);
//    glTexCoord2f (0.0, 1.0);
//    glVertex3f (0.0, 10.0, 10.0);
//    glEnd ();

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
   // printf("here %f %f %f \n", x,y,footprint_l);
    double mxheight=0;
    for(double i=x-footprint_l/2;i<x+footprint_l/2; i+=0.8 /*BS_MAX_LENGHT*/)
        for(double j=y-footprint_l/2;j<y+footprint_l/2; j+=0.8 /*OBS_MAX_LENGHT*/)
        {
            //printf("here %f %f %f \n", i,j,footprint_l);
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
