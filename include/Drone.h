#include "Visualizer.h"
#include "FeatureTracker.h"
#include "TooN/TooN.h"
#include "World.h"
#include "TSP.h"

#define     MAX_DIST_TO_OBSTACLES   5

using namespace TooN;
using namespace std;

struct PlanNode
{
    Vector<3> p;
    double interestingness;
    vector<PlanNode> children;
    bool expandable;
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
    void ToggleDrawInterestingness();

    void GeneratePlan();
    void ExecutePlan();
    void DestroyPlan();

    void GoToNextWP(double step_l);
    void ChangeSpeed(double ds);

private:

    bool SetExpandable(PlanNode &pn);
    void VisitWaypoint(PlanNode node);
    void GenerateCoveragePlan(double w_w, double w_l, double flying_height);
    void OnLevelPlanExecuted();

    FeatureTracker sensor;

    bool newPlan;
    bool executingPlan;
    int levels;
    double footprint_length;
    int curLevel;
    timeval scanStartTime;

    double speed;
    //executed path
    vector< PlanNode > pathWPs;
    vector< PlanNode > nextPath;

    vector<Entity*> tspoint;

    Vector<3> homePos;
    vector<Entity*> shortestPath;
    TSP tsp;

};
