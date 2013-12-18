#include "MeshCreator.h"
#include "GL/gl.h"

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
    double hR = 5;
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

        glColor3f(1-(p[0].z()+p[1].z()+p[2].z())/hR,1-(p[0].z()+p[1].z()+p[2].z())/hR,0.8);

        glVertex3f(p[0].x(), p[0].y(), p[0].z());
        glVertex3f(p[1].x(), p[1].y(), p[1].z());
        glVertex3f(p[2].x(), p[2].y(), p[2].z());
    }
    glEnd();


}
