#include "TooN/TooN.h"
#include <CNearTree.h>
#include <Visualizer.h>

struct Feature
{
    TooN::Vector<3, double> pos;
    double size;
};

class FeatureTracker: public Visualizer
{
public:
    FeatureTracker(TooN::Vector<3,double> startPos);
    ~FeatureTracker();

    void glDraw();

    void MoveSensor(TooN::Vector<3,double> dPos);

private:
    CNearTreeHandle treehandle;
};
