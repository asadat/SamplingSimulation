#include "Visualizer.h"
#include "FeatureTracker.h"
#include "TooN/TooN.h"
#include "World.h"
#include "TSP.h"

using namespace TooN;
using namespace std;

struct PlanNode
{
    Vector<3> p;
    double entropy;
    vector<PlanNode> children;
};

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
    void OnLevelPlanExecuted();

    bool newPlan;
    bool executingPlan;
    int levels;
    double footprint_length;
    timeval scanStartTime;

    //executed path
    vector< PlanNode > pathWPs;
    vector< PlanNode > nextPath;

    vector<Entity*> tspoint;

    Vector<3> homePos;
    vector<Entity*> shortestPath;
    TSP tsp;

};
