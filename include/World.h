#ifndef WORLD_
#define WORLD_

#include <GL/glut.h>
#include "Visualizer.h"
#include "TooN/TooN.h"
#include <vector>

#define RAND(a,b)    ((a)+((b)-(a))*((double)(rand()%1000))/1000)


using namespace TooN;
using namespace std;

struct Plane2D
{
    Vector<3, double> p1; // top-left corner;
    Vector<3, double> p2; // bottom-right corner;

    bool inside(Vector<3, double> p)
    {
        return (p1[0] <= p[0] && p2[0] >= p[0] && p1[1] >= p[1] && p2[1] <= p[1]);
    }

    bool inside(Vector<2, double> p)
    {
        return (p1[0] <= p[0] && p2[0] >= p[0] && p1[1] >= p[1] && p2[1] <= p[1]);
    }

    bool inside(double x, double y)
    {
        return (p1[0] <= x && p2[0] >= x && p1[1] >= y && p2[1] <= y);
    }
};

class World: public Visualizer
{
public:

    static World * Instance()
    {
        if(instance == NULL)
            instance = new World();

        return instance;
    }


    void glDraw();

    double GetHeight(double x, double y);
    void InsertPlane(double x1, double y1, double z1, double x2, double y2, double z2);
    void ToggleDraw();
    void PopulateWorld();
    double GetWidth();
    double GetLength();
    double GetMaxHeight();
    double GetMaxHeightInRect(double x, double y, double footprint_l);

private:

    static World * instance;
    World();

    void DrawBox(Vector<3,double> p1, Vector<3,double> p2);
    vector<Plane2D> planes;

    bool bDrawGhost;
    bool bDraw;

};

#endif
