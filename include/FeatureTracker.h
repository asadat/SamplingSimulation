#ifndef FEATURE_TRACKER_
#define FEATURE_TRACKER_

#include "TooN/TooN.h"
//#include <CNearTree.h>
#include <Visualizer.h>
#include "MeshCreator.h"


#define     DEG2RAD     3.14/180
#define     FOV         90.1*DEG2RAD

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
    void MoveSensorTo(TooN::Vector<3,double> pos);

    void ToggleDrawInterestingnessField(){bDrawInterestingness = !bDrawInterestingness;}
    void ToggleSensing(){ bSensing =! bSensing;}
    void TurnOnSensing(bool on){bSensing = on;}
    void ClearHistory();

    Vector<3, double> GetSensorPose(){return pose;}
    double GetSamplingLevels(int &level);
    void SetPose(Vector<3, double> newpose);

    double ResetInterestingness(){mesh.ResetMaxInterestingness();}
    double GetInterestingness(TooN::Vector<2, double> tl, TooN::Vector<2, double> br);
    double GetMaxInterestingness(){return 0.1;/*mesh.maxInterestingness;*/}
    double GetFootprint(double sensorHight);
    double GetHeightWithGootprint(double footprint_l);

private:

    void GenerateFeatures(int size, Vector<3, double> viewpoint);
    std::vector<Feature> TrackFeatures(TooN::Vector<3, double> viewpoint);
    void UpdateMatchedFeatures();
    bool InsideFOV(Feature f, Vector<3, double> pos);
    void OnPlanExecuted();


    TooN::Vector<3, double> pose;
    std::vector<Feature> features;
    std::vector<Feature> matchedFeatures;

    bool bSensing;
    MeshCreator mesh;
    //double footprint_length;
    bool bDrawInterestingness;

    vector<double> entropies;

};

#endif
