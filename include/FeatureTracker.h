#include "TooN/TooN.h"
#include <CNearTree.h>
#include <Visualizer.h>

#include "MeshCreator.h"

#define     DEG2RAD     3.14/180
#define     FOV         92*DEG2RAD

using namespace std;
using namespace TooN;

struct Feature
{
    Vector<3, double> pos;
    Vector<3, double> view;
    double size;
};

class FeatureTracker: public Visualizer
{
public:
    FeatureTracker(TooN::Vector<3,double> startPos);

    void glDraw();
    void DrawFrustum(Vector<3, double> camp, double size);

    void MoveSensor(TooN::Vector<3,double> dPos);

    void ToggleDrawEntropyField(){bDrawEntropy = !bDrawEntropy;}
    void ToggleSensing(){ bSensing =! bSensing;}
    void ClearHistory();

    void ExecuteCoveragePlan(double w_w, double w_l, double flying_height, double step_l);

    void GoToNextWP(double step_l);
    Vector<3, double> GetSensorPose(){return pose;}

private:

    void GenerateFeatures(int size, Vector<3, double> viewpoint);
    void SetPose(Vector<3, double> newpose);
    std::vector<Feature> TrackFeatures(TooN::Vector<3, double> viewpoint);
    void UpdateMatchedFeatures();
    bool InsideFOV(Feature f, Vector<3, double> pos);
    void OnPlanExecuted();


    TooN::Vector<3, double> pose;
    std::vector<Feature> features;
    std::vector<Feature> matchedFeatures;

    bool bSensing;
    MeshCreator mesh;
    double footprint_length;
    bool bDrawEntropy;

    //executed path
    vector< Vector<3, double> > pathWPs;
    vector< Vector<3, double> > nextPath;
    vector<double> entropies;


};

