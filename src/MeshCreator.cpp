#include "MeshCreator.h"
#include "GL/gl.h"

using namespace TooN;
MeshCreator::MeshCreator():Visualizer()
{

}


void MeshCreator::AddVertex(TooN::Vector<3,double> v)
{
    Point p(v[0], v[1], v[2]);

    mesh.insert(p);
}

void MeshCreator::CleanMesh()
{
    mesh.clear();
    maxEntropy = 0.001;
}

void MeshCreator::glDraw()
{
    Delaunay::Finite_faces_iterator fit;

    //WireFrame
    glColor3f(0.5,0.5,0.5);
    glLineWidth(2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    for(fit = mesh.finite_faces_begin();fit != mesh.finite_faces_end(); ++fit)
    {
        Point p[3];
        p[0] = fit->vertex(0)->point();
        p[1] = fit->vertex(1)->point();
        p[2] = fit->vertex(2)->point();

        glVertex3f(p[0].x(), p[0].y(), p[0].z());
        glVertex3f(p[1].x(), p[1].y(), p[1].z());
        glVertex3f(p[2].x(), p[2].y(), p[2].z());
    }
    glEnd();

    //Fill
    double hR = 10;
    glColor3f(0.8,0.6,1);
    glLineWidth(2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for(fit = mesh.finite_faces_begin();fit != mesh.finite_faces_end(); ++fit)
    {
        Point p[3];
        p[0] = fit->vertex(0)->point();
        p[1] = fit->vertex(1)->point();
        p[2] = fit->vertex(2)->point();

        double dz[3];
        dz[0] = fabs(p[0].z() - p[1].z());
        dz[1] = fabs(p[0].z() - p[2].z());
        dz[2] = fabs(p[2].z() - p[1].z());
        double cc = dz[0]+dz[1]+dz[2];

        //glColor3f(1-(p[0].z()+p[1].z()+p[2].z())/hR,1-(p[0].z()+p[1].z()+p[2].z())/hR,0.8);
        glColor3f(1-cc/hR,1-cc/hR,0.8);

        glVertex3f(p[0].x(), p[0].y(), p[0].z());
        glVertex3f(p[1].x(), p[1].y(), p[1].z());
        glVertex3f(p[2].x(), p[2].y(), p[2].z());
    }
    glEnd();
}

double MeshCreator::GetEntropy(Vector<2, double> tl, Vector<2, double> br)
{
    double entropy = 0;
    Delaunay::Finite_faces_iterator fit;

    for(fit = mesh.finite_faces_begin();fit != mesh.finite_faces_end(); ++fit)
    {
        Point p[3];
        p[0] = fit->vertex(0)->point();
        p[1] = fit->vertex(1)->point();
        p[2] = fit->vertex(2)->point();

        int n = 0;
        for(int i=0; i < 3; i++)
        {
            if(p[i].x() > tl[0] && p[i].x() < br[0] && p[i].y() < tl[1] && p[i].y() > br[1])
            {
                n++;
            }

            if(n>=2)
                break;
        }

        if(n>=2)
        {
            entropy += fabs(p[0].z()-p[1].z()) + fabs(p[1].z()-p[2].z()) + fabs(p[2].z()-p[0].z());
        }
    }

    maxEntropy = (maxEntropy>entropy)?maxEntropy:entropy;
   // printf("MAX: %f\n",max);
    return entropy;
}
