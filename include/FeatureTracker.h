#include "TooN/TooN.h"
#include <CNearTree.h>
#include <Visualizer.h>

#define     DEG2RAD     3.14/180
#define     FOV         60*DEG2RAD

using namespace std;
using namespace TooN;

struct Feature
{
    TooN::Vector<3, double> pos;
    double size;
};

class FeatureTracker: public Visualizer
{
public:
    FeatureTracker(TooN::Vector<3,double> startPos);

    void glDraw();

    void MoveSensor(TooN::Vector<3,double> dPos);

private:

    std::vector<Feature> MatchedFeatures(TooN::Vector<3, double> viewpoint);
    bool InsideFOV(Feature f, Vector<3, double> pos);


    TooN::Vector<3, double> pose;
    std::vector<Feature> features;
};
