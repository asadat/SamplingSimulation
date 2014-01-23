#include "Drone.h"
#include <sys/time.h>

int MyRand(int n)
{
    return std::rand() % n;
}

Drone::Drone():sensor(makeVector(0,0,3))
{
    strategy = BREADTH_FIRST;

    sensor.TurnOnSensing(false);
    sensor.ClearHistory();

    branching_deg = 2;
    speed = 2000;
    levels = 2;
    executingPlan = false;


//    for(int i=0; i<25;i++)
//    {
//        Entity* en = new Entity();
//        en->pos = makeVector(pow(-1,i)*10*sin(((double)i)*2*3.14/100.0),pow(-1,i)*10*cos(((double)i)*2*3.14/100.0),5);
//        en->nodeIdx = i;
//        en->start = false;
//        en->end = false;
//        tspoint.push_back(en);
//    }

//    tspoint.front()->start=true;
//    tspoint.back()->end = true;
//    std::random_shuffle(tspoint.begin(), tspoint.end(),MyRand);
//    shortestPath = tsp.GetShortestPath_heu(tspoint);
}

Drone::~Drone()
{

}

void Drone::init(int branchingDeg, int startlvl, Traverse_Strategy st)
{
    strategy = st;
    branching_deg = branchingDeg;
    levels = startlvl;
    GoToLevel(levels);
    GeneratePlan();
    ExecutePlan();
}

void Drone::glDraw()
{

    for(int i=0; i<tspoint.size(); i++)
    {
       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
        glColor3f(0,0,1);
        glPointSize(10);
        glBegin(GL_POINTS);

        if(tspoint[i]->start)
            glColor3f(0,1,0);
        else if(tspoint[i]->end)
            glColor3f(1,0,0);

        glVertex3f(tspoint[i]->pos[0], tspoint[i]->pos[1], tspoint[i]->pos[2]);
        glEnd();
    }

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

    //Draw Next WPs
    glColor3f(0,1,0);
    glPointSize(10);
    glBegin(GL_POINTS);
    for(int i=0; i<nextPath.size(); i++)
    {
        glVertex3f(nextPath[i].p[0], nextPath[i].p[1], nextPath[i].p[2]);
    }
    glEnd();


    for(int i=0; i<shortestPath.size(); i++)
    {
       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
        glColor3f(0,0,0);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3f(shortestPath[i]->pos[0], shortestPath[i]->pos[1], shortestPath[i]->pos[2]);
        glEnd();


        if(i+1<shortestPath.size())
        {
            glColor3f(0.01*((double)i),0,0);
            glLineWidth(3);
            glBegin(GL_LINES);
            glVertex3f(shortestPath[i]->pos[0],shortestPath[i]->pos[1],shortestPath[i]->pos[2]);
            glVertex3f(shortestPath[i+1]->pos[0],shortestPath[i+1]->pos[1],shortestPath[i+1]->pos[2]);
            glEnd();
        }
    }

    if(!pathWPs.empty())
    {
        Vector<3> wpp = pathWPs.front().p;
        double ftl = sensor.GetFootprint(wpp[2]);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor4f(0.6,0,1,0.2);

        double h = 0.01;
        glBegin(GL_POLYGON);
        glVertex3f(wpp[0]-ftl/2, wpp[1]-ftl/2, h);
        glVertex3f(wpp[0]+ftl/2, wpp[1]-ftl/2, h);
        glVertex3f(wpp[0]+ftl/2, wpp[1]+ftl/2, h);
        glVertex3f(wpp[0]-ftl/2, wpp[1]+ftl/2, h);
        glVertex3f(wpp[0]-ftl/2, wpp[1]-ftl/2, h);
        glEnd();
    }

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

        GoToNextWP(speed*(elapsedTime/1000.0));

        last_time = seconds;

    }
}

void Drone::MoveSensor(TooN::Vector<3,double> dPos)
{
    surveyLength += sqrt(dPos*dPos);
    sensor.MoveSensor(dPos);
}

Vector<3> Drone::GetPose()
{
    return sensor.GetSensorPose();
}

void Drone::ToggleDrawInterestingness()
{
    sensor.ToggleDrawInterestingnessField();
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

void Drone::GoToLevel(int lvl)
{
    Vector<3, double> p= GetPose();

    levels = lvl;
    p[2] = sensor.GetSamplingLevels(levels);
    sensor.SetPose(p);
}

void Drone::GeneratePlan()
{
    surveyLength = 0;
    DestroyPlan();
    GenerateCoveragePlan(World::Instance()->GetWidth(),
                 World::Instance()->GetLength(), sensor.GetSensorPose()[2]);
    //printf("plan generated./n");

}

void Drone::ExecutePlan()
{
    //sensor.TurnOnSensing(true);
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

double Drone::PathLength(vector< PlanNode > & path)
{
    double sum=0;
    for(int i=0; i+1 < path.size(); i++)
    {
        Vector<3> dp = path[i+1].p - path[i].p;
        sum += sqrt(dp*dp);
    }

    return sum;
}

void Drone::GenerateCoveragePlan(double w_w, double w_l, double flying_height)
{
    sensor.TurnOnSensing(false);

    curLevel = 1;
    double footPrint_l = sensor.GetFootprint(flying_height);
    footprint_length = footPrint_l;

    Vector<3, double> startPoint = makeVector(-w_w/2, -w_l/2, flying_height);
    sensor.SetPose(startPoint+ makeVector(footPrint_l/2,footPrint_l/2,0));
    homePos = startPoint;
    homePos[2] = 0;

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
//            if(false && !pathWPs.empty())
//            {
//                PlanNode pn;
//                pn.expandable = true;
//                Vector<3,double>  prevWP = (pathWPs.back().p);
//                if(fabs(scanHeight-prevWP[2])<0.001)
//                {
//                    pn.p = makeVector(x,y, scanHeight);
//                    pathWPs.push_back(pn);
//                }
//                else if(prevWP[2] < scanHeight)
//                {
//                    pn.p = makeVector(prevWP[0],prevWP[1], scanHeight);
//                    pathWPs.push_back(pn);

//                    pn.p = makeVector(x,y, scanHeight);
//                    pathWPs.push_back(pn);
//                }
//                else
//                {
//                    pn.p = makeVector(x,y, prevWP[2]);
//                    pathWPs.push_back(pn);

//                    pn.p = makeVector(x,y, scanHeight);
//                    pathWPs.push_back(pn);
//                }
//            }
//            else
            {
                PlanNode pn;
                pn.p = makeVector(x,y, scanHeight);
                SetExpandable(pn);
                pathWPs.push_back(pn);
            }

//            PlanNode tmp;
//            int child_n = 3;
//            double next_fp = footPrint_l/child_n;
//            double xx = x-footPrint_l/2.0 + next_fp/2.0;
//            double yy = y-footPrint_l/2.0 + next_fp/2.0;

//            double zz = sensor.GetHeightWithGootprint(next_fp);

//            for(int i=0; i<child_n; i++)
//                for(int j=0; j<child_n; j++)
//                {
//                    tmp.p = makeVector(xx + i*next_fp, yy + j*next_fp, zz);
//                    nextPath.push_back(tmp);
//                }
            /*
            tmp.p = makeVector(x+footPrint_l/4,y+footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
            tmp.p = makeVector(x+footPrint_l/4,y-footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
            tmp.p = makeVector(x-footPrint_l/4,y+footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);
            tmp.p = makeVector(x-footPrint_l/4,y-footPrint_l/4, flying_height/2);
            nextPath.push_back(tmp);*/
        }

    }

    //surveyLength += PathLength(pathWPs);

    PlanNode pn;
    pn.expandable = false;
    pn.p = startPoint;
    pn.homeNode = true;
    pathWPs.push_back(pn);


}

void Drone::ChangeSpeed(double ds)
{
    speed = (speed+ds > 0)? speed+ds : speed;
}

void Drone::GoToNextWP(double step_l)
{
    bool flag = false;

    for(int i=0; i < pathWPs.size(); i++)
    {
        flag = true;
        PlanNode goalNode = pathWPs[i];
        double obsHeight = World::Instance()->GetMaxHeightInRect(goalNode.p[0],goalNode.p[1],sensor.GetFootprint(goalNode.p[2]));
        if(goalNode.p[2] - obsHeight < MAX_DIST_TO_OBSTACLES)
            goalNode.p[2] = obsHeight + MAX_DIST_TO_OBSTACLES;


        double wpd = sqrt((goalNode.p-GetPose())*(goalNode.p-GetPose()));
        //for(int j =0; j< floor(wpd/step_l); j++)
        {
            //printf("%f -/n",wpd);

            double dxyz = 0.001;
            Vector<3> toWP = makeVector(0.1,0.1,0.1);

            if(wpd > 0.001)
            {
                  dxyz = (step_l/wpd);
                  if(dxyz > 1)
                      dxyz = 1;

                  toWP = goalNode.p-GetPose();
            }

            MoveSensor(dxyz*toWP);
        }

       // MoveSensorTo(pathWPs[i+1]);

//        if(pathWPs.size()==1 && nextPath.empty())
//        {
//            sensor.TurnOnSensing(false);
//        }

        if(wpd < step_l || (pathWPs.size()<=1 && !nextPath.empty()))
        {
            if(pathWPs.size() == 1)
            {
                VisitWaypoint(pathWPs.front());

                //printf("level cleared.\n");
                pathWPs.clear();
                OnLevelPlanExecuted();
            }
            else
            {
                VisitWaypoint(pathWPs.front());
                pathWPs.erase(pathWPs.begin());

                if(pathWPs.empty())
                {
                    exit(0);
                }

                //Starting to go back home
                if(pathWPs.size() == 1 && nextPath.empty())
                {
                    sensor.TurnOnSensing(false);
                    pathWPs[0].p[2] = GetPose()[2];
                    //add the cost of returning home to the total path length
                    //Vector<3> dp = pathWPs[0].p-GetPose();
                    //surveyLength += sqrt(dp*dp);

                }

                //determin next waypoint : consider obstacles

            }
        }

        break;
    }

    if(!flag)
    {
        printf("%d %f\n", World::Instance()->GetNumOfIntCells(), surveyLength);
        exit(0);
    }
}

bool Drone::SetExpandable(PlanNode &pn)
{
    double dh = pn.p[2] - World::Instance()->GetMaxHeightInRect(pn.p[0], pn.p[1], sensor.GetFootprint(pn.p[2]));
    if(dh >  MAX_DIST_TO_OBSTACLES)
    {
       // printf("dh ====  %f\n", dh);
        pn.expandable = true;
    }
    else
        pn.expandable = false;

}

void Drone::VisitWaypoint(PlanNode node)
{
    if(!node.expandable)
        return;

    PlanNode tmp;
    int child_n = branching_deg;
    double footPrint_l = sensor.GetFootprint(node.p[2]);
    double next_fp = footPrint_l/child_n;
    double xx = node.p[0]-footPrint_l/2.0 + next_fp/2.0;
    double yy = node.p[1]-footPrint_l/2.0 + next_fp/2.0;
    double zz = sensor.GetHeightWithGootprint(next_fp);

  //  printf("1\n");
    for(int i=0; i<child_n; i++)
        for(int j=0; j<child_n; j++)
        {
            tmp.p = makeVector(xx + i*next_fp, yy + j*next_fp, zz);
            double dh = tmp.p[2] - World::Instance()->GetMaxHeightInRect(tmp.p[0], tmp.p[1], sensor.GetFootprint(tmp.p[2]));
            if(dh <  MAX_DIST_TO_OBSTACLES)
            {
               // printf("dh ====  %f\n", dh);
                continue;
            }

            double footprint_l = sensor.GetFootprint(tmp.p[2]);
            Vector<2, double > tl = makeVector(tmp.p[0]-footprint_l/2, tmp.p[1]+footprint_l/2);
            Vector<2, double > rb = makeVector(tmp.p[0]+footprint_l/2, tmp.p[1]-footprint_l/2);
            tmp.interestingness = sensor.GetInterestingness(tl,rb)/sensor.GetMaxInterestingness();

            SetExpandable(tmp);
            if(tmp.interestingness > 0.2)
            {
                if(strategy == BREADTH_FIRST)
                {
                    nextPath.push_back(tmp);
                }
                else if(strategy == DEPTH_FIRST)
                {
                    pathWPs.insert(pathWPs.begin()+1,tmp);
                }
            }
        }
}

void Drone::OnLevelPlanExecuted()
{
    timeval scanEnd;
    gettimeofday(&scanEnd,NULL);

    double totalTime = (-scanStartTime.tv_sec + scanEnd.tv_sec);      // sec to ms
    totalTime += (-scanStartTime.tv_usec + scanEnd.tv_usec) / 1000000.0;   // us to ms


    //printf("Total Time:%f \n", totalTime);

    if(nextPath.empty())
    {
        return;
    }


    sensor.ResetInterestingness();
    curLevel++;
//    for(int i=0; i< nextPath.size(); i++)
//    {
//        double footprint_l = sensor.GetFootprint(nextPath[i].p[2]);
//        Vector<2, double > tl = makeVector(nextPath[i].p[0]-footprint_l/2, nextPath[i].p[1]+footprint_l/2);
//        Vector<2, double > rb = makeVector(nextPath[i].p[0]+footprint_l/2, nextPath[i].p[1]-footprint_l/2);
//        nextPath[i].interestingness = sensor.GetInterestingness(tl,rb);
//    }

    tspoint.clear();

    Entity * en = new Entity();
    en->pos = GetPose();
    en->start = true;
    en->nodeIdx = -1;
    tspoint.push_back(en);

    int tspGoalIdx = 0;
    double tspGoalDist = 99999999;

    for(int i=0; i<nextPath.size();i++)
    {
        //printf("%f ", nextPath[i].interestingness/sensor.GetMaxInterestingness());
       /// if(nextPath[i].interestingness/sensor.GetMaxInterestingness() > /*(curLevel-1)**/0.2)
        {
            //printf("%f ",nextPath[i].entropy/sensor.GetMaxEntropy());
            en = new Entity();
            en->pos = nextPath[i].p;
            en->nodeIdx = i;
            tspoint.push_back(en);

            double dist2Goal = sqrt((homePos-en->pos)*(homePos-en->pos));
            //printf("d:%f\n", dist2Goal);
            if(dist2Goal<tspGoalDist)
            {
                tspGoalIdx = tspoint.size()-1;
                tspGoalDist = dist2Goal;
                //printf("d:%f i:%d\n", tspGoalDist,tspGoalIdx);
            }
        }
    }

    /*printf("start:%f\t%f\t%f\tend:%f\t%f\t%f\n", tspoint[0]->pos[0], tspoint[0]->pos[1], tspoint[0]->pos[2],tspoint[tspGoalIdx]->pos[0],
           tspoint[tspGoalIdx]->pos[1], tspoint[tspGoalIdx]->pos[2] );
*/
    en = new Entity();
    en->pos = homePos;
    en->pos[2]=tspoint.back()->pos[2];
    en->end = true;
    en->nodeIdx = -2;
    //tspoint.push_back(en);

    //tspoint[tspGoalIdx]->end = true;

//    for(int i=0;i<tspoint.size();i++)
//        printf("%d-",tspoint[i]->nodeIdx);
//    printf("\n");

    shortestPath.clear();
    //shortestPath = tsp.GetShortestPath_heu(tspoint);
    shortestPath = tsp.GetShortestPath(tspoint);
    shortestPath.push_back(en);

//    for(int i=0;i<shortestPath.size();i++)
//        printf("%d-",shortestPath[i]->nodeIdx);
//    printf("\n");

  /*  printf("start:%f\t%f\t%f\tend:%f\t%f\t%f\n", shortestPath[0]->pos[0], shortestPath[0]->pos[1], shortestPath[0]->pos[2],
           shortestPath.back()->pos[0],
           shortestPath.back()->pos[1], shortestPath.back()->pos[2] );
*/
    for(int i=0; i<shortestPath.size();i++)
    {
        if(shortestPath[i]->nodeIdx >= 0)
        {
            pathWPs.push_back(nextPath[shortestPath[i]->nodeIdx]);
        }
        else if(shortestPath[i]->nodeIdx == -2)
        {
            //surveyLength += PathLength(pathWPs);
            PlanNode pn;
            pn.p = homePos;
            pn.homeNode = true;
            double height;
            if(!pathWPs.empty())
            {
                height = pathWPs.back().p[2];
            }
            else
                height = GetPose()[2];

            pn.p[2] = height;

            pathWPs.push_back(pn);

        }
        if(shortestPath[i]->nodeIdx == -1)
        {
            PlanNode pn;
            pn.p = GetPose();
            pathWPs.push_back(pn);
        }
    }

    tspoint.clear();
    shortestPath.clear();
    nextPath.clear();

   // printf("#POI: %d\n",pathWPs.size());
}
