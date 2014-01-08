#include "Visualizer.h"
#include "FeatureTracker.h"
#include "TooN/TooN.h"
#include "World.h"
#include "TSP.h"

using namespace TooN;
using namespace std;

class Drone:public Visualizer
{
public:
    Drone();
    ~Drone();

    void glDraw();
    void Update();

    void MoveSensor(TooN::Vector<3,double> dPos);
    Vector<3> GetPose();
    void GoLevelDown();
    void GoLevelUp();
    void ToggleStealthMode();
    void ToggleDrawEntropy();

    void GeneratePlan();
    void ExecutePlan();
    void DestroyPlan();

    void GoToNextWP(double step_l);

    FeatureTracker sensor;
private:

    void GenerateCoveragePlan(double w_w, double w_l, double flying_height);

    bool newPlan;
    bool executingPlan;
    int levels;


    //executed path
    vector< Vector<3, double> > pathWPs;
    vector< Vector<3, double> > nextPath;

    vector<Entity*> shortestPath;
    TSP tsp;

};
