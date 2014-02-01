#ifndef WORLD_
#define WORLD_

#include <GL/glut.h>
#include "Visualizer.h"
#include "TooN/TooN.h"
#include <vector>
#include "MeshCreator.h"
#include "cvd/image.h"
#include "cvd/image_io.h"

#define RAND(a,b)    ((a)+((b)-(a))*((double)(rand()%1000))/1000)


using namespace TooN;
using namespace std;

struct Plane2D
{
    bool floor;
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
    void InsertPlane(double x1, double y1, double z1, double x2, double y2, double z2, bool floor=false);
    void InsertVisPlane(double x1, double y1, double z1, double x2, double y2, double z2);

    void ToggleDraw();
    void PopulateWorld(int int_per, int int_cells=-1);
    void PopulateWorldFromImage(char * imagepath);
    double GetWidth();
    double GetLength();
    double GetMaxHeight();
    double GetMaxHeightInRect(double x, double y, double footprint_l);
    double GetInterestingness(TooN::Vector<2, double> tl, TooN::Vector<2, double> br);
    int GetNumOfIntCells(){return inter_cells_n;}
    int GetPercOfIntCells(){return inter_perc;}
    static unsigned int textureName;

private:


    static World * instance;
    World();

    void DrawBox(Vector<3,double> p1, Vector<3,double> p2, bool floor);
    vector<Plane2D> planes;
    vector<Plane2D> vis_planes;


    bool bDrawGhost;
    bool bDraw;
    MeshCreator mesh;
    int inter_cells_n;
    int inter_perc;
    CVD::Image<CVD::Rgb<CVD::byte> > texture_img;


};

#endif
