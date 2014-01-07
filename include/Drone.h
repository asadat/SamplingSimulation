#include "Visualizer.h"
#include "FeatureTracker.h"
#include "TooN/TooN.h"

using namespace TooN;
using namespace std;

class Drone:public Visualizer
{
public:
    Drone();
    ~Drone();

    void glDraw();

 FeatureTracker sensor;
private:

};
