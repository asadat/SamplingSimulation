#include "Drone.h"
#include <sys/time.h>

Drone::Drone():sensor(makeVector(0,0,3))
{
    levels = 0;
    executingPlan = false;
}

Drone::~Drone()
{

}

void Drone::glDraw()
{
    // draw path
    for(int i=0; i<pathWPs.size(); i++)
    {
       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
        glColor3f(1,1,1);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3f(pathWPs[i].p[0], pathWPs[i].p[1], pathWPs[i].p[2]);
        glEnd();


        if(i+1<pathWPs.size())
        {
            glColor3f(0,0,0);
            glLineWidth(3);
            glBegin(GL_LINES);
            glVertex3f(pathWPs[i].p[0],pathWPs[i].p[1],pathWPs[i].p[2]);
            glVertex3f(pathWPs[i+1].p[0],pathWPs[i+1].p[1],pathWPs[i+1].p[2]);
            glEnd();
        }
    }


//    for(int i=0; i<shortestPath.size(); i++)
//    {
//       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
//        glColor3f(0,1,0);
//        glPointSize(10);
//        glBegin(GL_POINTS);
//        glVertex3f(shortestPath[i]->pos[0], shortestPath[i]->pos[1], shortestPath[i]->pos[2]);
//        glEnd();


//        if(i+1<shortestPath.size())
//        {
//            glColor3f(0.1*((double)i),0,0);
//            glLineWidth(3);
//            glBegin(GL_LINES);
//            glVertex3f(shortestPath[i]->pos[0],shortestPath[i]->pos[1],shortestPath[i]->pos[2]);
//            glVertex3f(shortestPath[i+1]->pos[0],shortestPath[i+1]->pos[1],shortestPath[i+1]->pos[2]);
//            glEnd();
//        }
//    }

}

void Drone::Update()
{
    static timeval last_time;
    static bool firstFrame = true;

    if(executingPlan)
    {
        timeval seconds;
        gettimeofday(&seconds, NULL);
        //printf("seconds: %f /n", seconds);

        double elapsedTime = (-last_time.tv_sec + seconds.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (-last_time.tv_usec + seconds.tv_usec) / 1000.0;   // us to ms

        if(elapsedTime/1000.0 > 0.5)
           firstFrame = true;

        if(firstFrame || newPlan)
        {
            newPlan = false;
            //printf("First Time /n");
            last_time = seconds;
            firstFrame = false;
        }

        elapsedTime = (-last_time.tv_sec + seconds.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (-last_time.tv_usec + seconds.tv_usec) / 1000.0;   // us to ms

        GoToNextWP(20.0*(elapsedTime/1000.0));

        last_time = seconds;

    }
}

void Drone::MoveSensor(TooN::Vector<3,double> dPos)
{
    sensor.MoveSensor(dPos);
}

Vector<3> Drone::GetPose()
{
    return sensor.GetSensorPose();
}

void Drone::ToggleDrawEntropy()
{
    sensor.ToggleDrawEntropyField();
}

void Drone::ToggleStealthMode()
{
    sensor.ToggleSensing();
}

void Drone::GoLevelDown()
{
    Vector<3, double> p= GetPose();

    p[2] = sensor.GetSamplingLevels(++levels);

    sensor.SetPose(p);
}

void Drone::GoLevelUp()
{
    Vector<3, double> p= GetPose();

    p[2] = sensor.GetSamplingLevels(--levels);

    sensor.SetPose(p);
}

void Drone::GeneratePlan()
{
    DestroyPlan();
    GenerateCoveragePlan(World::Instance()->GetWidth(),
                 World::Instance()->GetLength(), sensor.GetSensorPose()[2]);
    //printf("plan generated./n");

}

void Drone::ExecutePlan()
{
    sensor.TurnOnSensing(true);
    newPlan = true;
    executingPlan = true;
    gettimeofday(&scanStartTime, NULL);

}

void Drone::DestroyPlan()
{
    executingPlan = false;
    sensor.ClearHistory();
    pathWPs.clear();
    nextPath.clear();
}

void Drone::GenerateCoveragePlan(double w_w, double w_l, double flying_height)
{
    sensor.TurnOnSensing(false);

    double footPrint_l = 2*flying_height*tan(FOV/2);
    footprint_length = footPrint_l;

    Vector<3, double> startPoint = makeVector(-w_w/2, -w_l/2, flying_height);
    sensor.SetPose(startPoint+ makeVector(footPrint_l/2,footPrint_l/2,0));

    pathWPs.clear();
    nextPath.clear();
    //printf("start point: %f %f %f", pose[0], pose[1], pose[2]);

    bool alternateFlag = false;
    for(int i=0; i< ceil(w_w/footPrint_l); i++)
    {
        double x = startPoint[0] + footPrint_l/2 + ((double)i)*footPrint_l;

        alternateFlag = !alternateFlag;

        for(int j=(alternateFlag)?0:(ceil(w_l/footPrint_l)-1); (alternateFlag)?(j<ceil(w_l/footPrint_l)):(j>=0);(alternateFlag)?(j++):(j--))
        {
            double y = startPoint[1] + footPrint_l/2 + ((double)j)*footPrint_l;

            double scanHeight = flying_height;//+GetMaxHeightInFootprint(x,y,footPrint_l);//World::Instance()->GetHeight(x,y);
            if(false && !pathWPs.empty())
            {
                PlanNode pn;
                Vector<3,double>  prevWP = (pathWPs.back().p);
                if(fabs(scanHeight-prevWP[2])<0.001)
                {
                    pn.p = makeVector(x,y, scanHeight);
                    pathWPs.push_back(pn);
                }
                else if(prevWP[2] < scanHeight)
                {
                    pn.p = makeVector(prevWP[0],prevWP[1], scanHeight);
                    pathWPs.push_back(pn);

                    pn.p = makeVector(x,y, scanHeight);
                    pathWPs.push_back(pn);
                }
                else
                {
                    pn.p = makeVector(x,y, prevWP[2]);
                    pathWPs.push_back(pn);

                    pn.p = makeVector(x,y, scanHeight);
                    pathWPs.push_back(pn);
                }
            }
            else
            {
                PlanNode pn;
                pn.p = makeVector(x,y, scanHeight);
                pathWPs.push_back(pn);
            }

            PlanNode tmp;
            tmp.p = makeVector(x+footPrint_l/4,y+footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
            tmp.p = makeVector(x+footPrint_l/4,y-footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
            tmp.p = makeVector(x-footPrint_l/4,y+footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
            tmp.p = makeVector(x-footPrint_l/4,y-footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
        }

    }

}

void Drone::GoToNextWP(double step_l)
{
    for(int i=0; i < pathWPs.size(); i++)
    {
        double wpd = sqrt((pathWPs[i].p-GetPose())*(pathWPs[i].p-GetPose()));
        //for(int j =0; j< floor(wpd/step_l); j++)
        {
            //printf("%f -/n",wpd);

            double dxyz = 0.001;
            Vector<3> toWP = makeVector(0.1,0.1,0.1);

            if(wpd > 0.0000001)
            {
                  dxyz = (step_l/wpd);
                  toWP = pathWPs[i].p-GetPose();
            }

            MoveSensor(dxyz*toWP);
        }

       // MoveSensorTo(pathWPs[i+1]);

        if(wpd < step_l)
        {
            if(pathWPs.size() == 1)
            {
                //printf("level cleared.\n");
                pathWPs.clear();
                OnLevelPlanExecuted();
            }
            else
            {
                pathWPs.erase(pathWPs.begin());
            }
        }

        break;
    }
}

void Drone::OnLevelPlanExecuted()
{
    timeval scanEnd;
    gettimeofday(&scanEnd,NULL);

    double totalTime = (-scanStartTime.tv_sec + scanEnd.tv_sec);      // sec to ms
    totalTime += (-scanStartTime.tv_usec + scanEnd.tv_usec) / 1000000.0;   // us to ms


    printf("Total Time:%f \n", totalTime);

    if(nextPath.empty())
    {
        return;
    }

    double footprint_l = footprint_length/2;

    for(int i=0; i< nextPath.size(); i++)
    {
        Vector<2, double > tl = makeVector(nextPath[i].p[0]-footprint_l/2, nextPath[i].p[1]+footprint_l/2);
        Vector<2, double > rb = makeVector(nextPath[i].p[0]+footprint_l/2, nextPath[i].p[1]-footprint_l/2);
        nextPath[i].entropy = sensor.GetEntropy(tl,rb);
    }

    vector<Entity*> tspoint;

    Entity * en = new Entity();
    en->pos = GetPose();
    en->start = true;
    en->nodeIdx = -1;
    tspoint.push_back(en);

    for(int i=0; i<nextPath.size();i++)
    {
        if(nextPath[i].entropy/sensor.GetMaxEntropy() > 0.2)
        {
            //printf("%f ",nextPath[i].entropy/sensor.GetMaxEntropy());
            en = new Entity();
            en->pos = nextPath[i].p;
            en->nodeIdx = i;
            tspoint.push_back(en);
        }
    }

    //printf("\n");

    shortestPath.clear();
    shortestPath = tsp.GetShortestPath(tspoint);

    for(int i=0; i<shortestPath.size();i++)
    {
        if(shortestPath[i]->nodeIdx >= 0)
        {
            pathWPs.push_back(nextPath[shortestPath[i]->nodeIdx]);
        }
    }

    nextPath.clear();

}
