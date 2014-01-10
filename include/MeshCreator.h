#ifndef MESH_CREATOR_
#define MESH_CREATOR_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include "Visualizer.h"
#include "TooN/TooN.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Projection_traits_xy_3<K> Gt;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay;
typedef K::Point_3 Point;

class MeshCreator : public Visualizer
{
public:
    MeshCreator();

    void CleanMesh();
    void AddVertex(TooN::Vector<3, double> v);

    void glDraw();

    double GetInterestingness(TooN::Vector<2, double> tl, TooN::Vector<2, double> br);

    double maxInterestingness;
private:

    Delaunay mesh;

};

#endif
