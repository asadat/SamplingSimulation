#include "Drone.h"
#include <sys/time.h>

int MyRand(int n)
{
    return std::rand() % n;
}

Drone::Drone():sensor(makeVector(0,0,3))
{
    //printf("hello\n");
    tree.CreateTree();
    //printf("he\n");
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
    tree.Destroy();
}

void Drone::init(int branchingDeg, int startlvl, Traverse_Strategy st)
{
    strategy = st;
    branching_deg = branchingDeg;
    startLevel = startlvl;
    levels = startlvl;
    GoToLevel(levels);
    GeneratePlan();
    ExecutePlan();
}

void Drone::glDraw()
{
    glDisable(GL_TEXTURE_2D);
//    for(int i=0; i<tspoint.size(); i++)
//    {
//       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
//        glColor3f(0,0,1);
//        glPointSize(10);
//        glBegin(GL_POINTS);

//        if(tspoint[i]->start)
//            glColor3f(0,1,0);
//        else if(tspoint[i]->end)
//            glColor3f(1,0,0);

//        glVertex3f(tspoint[i]->pos[0], tspoint[i]->pos[1], tspoint[i]->pos[2]);
//        glEnd();
//    }

   tree.glDraw();

//return;
    // draw path
//    for(int i=0; i<pathWPs.size(); i++)
//    {
//       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
//        if(pathWPs[i]->tentative == 1)
//            glColor3f(1,0,0);
//        else
//            glColor3f(1,1,1);

//        glPointSize(9);
//        glBegin(GL_POINTS);
//        glVertex3f(pathWPs[i]->p[0], pathWPs[i]->p[1], pathWPs[i]->p[2]);
//        glEnd();


//        //glLineStipple(5, 0xAAAA);
//        //glEnable(GL_LINE_STIPPLE);

//        if(i+1<pathWPs.size())
//        {
//            glColor3f(0.1,0.1,0.5);
//            glLineWidth(9);
//            glBegin(GL_LINES);
//            glVertex3f(pathWPs[i]->p[0],pathWPs[i]->p[1],pathWPs[i]->p[2]);
//            glVertex3f(pathWPs[i+1]->p[0],pathWPs[i+1]->p[1],pathWPs[i+1]->p[2]);
//            glEnd();
//        }
//        glDisable(GL_LINE_STIPPLE);
//    }

    for(int i=0; i<wps.size(); i++)
    {
        glColor3f(1,0.5,0.5);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3f(wps[i]->p[0], wps[i]->p[1], wps[i]->p[2]);
        glEnd();

        if(i+1<wps.size())
        {
            glColor3f(0.1,0.1,0.5);
            glLineWidth(3);
            glBegin(GL_LINES);
            glVertex3f(wps[i]->p[0],wps[i]->p[1],wps[i]->p[2]);
            glVertex3f(wps[i+1]->p[0],wps[i+1]->p[1],wps[i+1]->p[2]);
            glEnd();
        }

    }

    //Draw Next WPs
//    glColor3f(0,1,0);
//    glPointSize(10);
//    glBegin(GL_POINTS);
//    for(int i=0; i<nextPath.size(); i++)
//    {
//        glVertex3f(nextPath[i]->p[0], nextPath[i]->p[1], nextPath[i]->p[2]);
//    }
//    glEnd();


//    for(int i=0; i<shortestPath.size(); i++)
//    {
//       // DrawFrustum(pathWPs[i], pathWPs[i][2]);
//        glColor3f(0,0,0);
//        glPointSize(10);
//        glBegin(GL_POINTS);
//        glVertex3f(shortestPath[i]->pos[0], shortestPath[i]->pos[1], shortestPath[i]->pos[2]);
//        glEnd();


//        if(i+1<shortestPath.size())
//        {
//            glColor3f(0.01*((double)i),0,0);
//            glLineWidth(3);
//            glBegin(GL_LINES);
//            glVertex3f(shortestPath[i]->pos[0],shortestPath[i]->pos[1],shortestPath[i]->pos[2]);
//            glVertex3f(shortestPath[i+1]->pos[0],shortestPath[i+1]->pos[1],shortestPath[i+1]->pos[2]);
//            glEnd();
//        }
//    }

    //DrawCell(pathWPs.front());
//    if(!pathWPs.empty())
//    {
//        Vector<3> wpp = pathWPs.front()->p;
//        double ftl = sensor.GetFootprint(wpp[2]);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//        if( pathWPs.front()->children.empty())
//        {
//            glColor4f(0.6,0,1,0.2);
//            double h = 0.01;
//            glBegin(GL_POLYGON);
//            glVertex3f(wpp[0]-ftl/2, wpp[1]-ftl/2, h);
//            glVertex3f(wpp[0]+ftl/2, wpp[1]-ftl/2, h);
//            glVertex3f(wpp[0]+ftl/2, wpp[1]+ftl/2, h);
//            glVertex3f(wpp[0]-ftl/2, wpp[1]+ftl/2, h);
//            glVertex3f(wpp[0]-ftl/2, wpp[1]-ftl/2, h);
//            glEnd();
//        }
//        else
//        {



//        }
//    }

}

void Drone::DrawCell(PlanNode *p)
{
    Vector<3> wpp = p->p;
    double ftl = sensor.GetFootprint(wpp[2]);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(!p->children.empty())
    {
        for(int i=0; i< p->children.size(); i++)
        {
            Vector<3> wpp1 = p->children[i]->p;
            double ftl1 = sensor.GetFootprint(wpp1[2]);

            if(p->children[i]->interestingness > 0.2)
              glColor4f(0.1,0,1,0.2);
            else
              glColor4f(0.9,0.3,1,0.2);

            double h = 0.01;
            glBegin(GL_POLYGON);
            glVertex3f(wpp1[0]-ftl1/2, wpp1[1]-ftl1/2, h);
            glVertex3f(wpp1[0]+ftl1/2, wpp1[1]-ftl1/2, h);
            glVertex3f(wpp1[0]+ftl1/2, wpp1[1]+ftl1/2, h);
            glVertex3f(wpp1[0]-ftl1/2, wpp1[1]+ftl1/2, h);
            glVertex3f(wpp1[0]-ftl1/2, wpp1[1]-ftl1/2, h);
            glEnd();
        }
    }
    else
    {
        if(p->interestingness > 0.2)
          glColor4f(0.1,0,1,0.2);
        else
          glColor4f(0.9,0.3,1,0.2);

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


        MoveToGoal(speed*(elapsedTime/1000.0));

        last_time = seconds;

    }
}

void Drone::MoveSensor(TooN::Vector<3,double> dPos)
{
    Vector<3> dp = dPos;
//    surveyLength += 2*dPos[2];
//    dPos[2] = 0;
    surveyLength += sqrt(dPos*dPos);

    sensor.MoveSensor(dp);
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
    //printf("plan generated.\n");

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
    wps.clear();
    executingPlan = false;
    sensor.ClearHistory();

    for(int i=0; i<pathWPs.size(); i++)
    {
        delete pathWPs[i];
    }
    pathWPs.clear();

    for(int i=0; i<nextPath.size(); i++)
    {
        delete nextPath[i];
    }
    nextPath.clear();

    tree.Destroy();
}

void Drone::AddChild(PlanNode* parent, PlanNode* child)
{
    if(child==NULL || parent==NULL)
    {
        printf("Either child or parent is null!\n");
    }

   // parent->children.push_back(child);
   // child->parent = parent;

    child->depth = parent->depth+1;
}


double Drone::PathLength(vector< PlanNode* > & path)
{
    double sum=0;
    for(int i=0; i+1 < path.size(); i++)
    {
        Vector<3> dp = path[i+1]->p - path[i]->p;
        sum += sqrt(dp*dp);
    }

    return sum;
}

void Drone::GenerateCoveragePlan(double w_w, double w_l, double flying_height)
{
    sensor.TurnOnSensing(false);
    if(strategy == LAWNMOWER)
    {
        double fh = flying_height;
        double fp = sensor.GetFootprint(flying_height);
        while(fh > MAX_DIST_TO_OBSTACLES)
        {
            fp = fp/branching_deg;
            fh = sensor.GetHeightWithGootprint(fp);
        }

        fh = sensor.GetHeightWithGootprint(fp*branching_deg);
        flying_height = fh;
    }



    curLevel = 1;
    double footPrint_l = sensor.GetFootprint(flying_height);
    footprint_length = footPrint_l;

    Vector<3, double> startPoint = makeVector(-w_w/2, -w_l/2, flying_height);
    //sensor.SetPose(startPoint+ makeVector(footPrint_l/2,footPrint_l/2,0));
    sensor.SetPose(startPoint);
    homePos = startPoint;
    homePos[2] = 0;

    pathWPs.clear();
    nextPath.clear();
   // printf("start point: %f %f %f", pose[0], pose[1], pose[2]);

   // printf("here3\n");

    tree.CreateTree();
    tree.root->p = makeVector(0,0,sensor.GetHeightWithGootprint(w_w));

    //PlanNode * root = CreatePlanNode();
    //root->visited = true;
    //root->parent = NULL;

    bool alternateFlag = false;
    for(int i=0; i< ceil(w_w/footPrint_l); i++)
    {
        double x = startPoint[0] + footPrint_l/2 + ((double)i)*footPrint_l;

        alternateFlag = !alternateFlag;

        for(int j=(alternateFlag)?0:(ceil(w_l/footPrint_l)-1); (alternateFlag)?(j<ceil(w_l/footPrint_l)):(j>=0);(alternateFlag)?(j++):(j--))
        {

            double y = startPoint[1] + footPrint_l/2 + ((double)j)*footPrint_l;

            double scanHeight = flying_height;

            PlanNode *pn = CreatePlanNode();
            pn->p = makeVector(x,y, scanHeight);

           // printf("here1\n", i, j);

            SetExpandable(pn);

            //printf("here2\n", i, j);

            pathWPs.push_back(pn);
            tree.AddChild(tree.root, pn);
        }

    }


    int lvl = 1;
    while(tree.levelNodes.find(lvl) != tree.levelNodes.end())
    {
        vector<PlanNode*> & lvlnds = tree.levelNodes[lvl];
        for(int i=0; i<lvlnds.size();i++)
        {
            GenerateSubtree(lvlnds[i],1);
        }

        lvl++;
    }

//    printf("here2\n");
//    PlanNode *pn= CreatePlanNode();
//    pn->expandable = false;
//    pn->p = startPoint;
//    pn->homeNode = true;
//    pathWPs.push_back(pn);


}

void Drone::ChangeSpeed(double ds)
{
    speed = (speed+ds > 0)? speed+ds : speed;
}

PlanNode * Drone::CreatePlanNode()
{
    PlanNode * pn = new PlanNode();
    pn->tentative = -1;
    pn->parent = NULL;
    return pn;
}

void Drone::SortNodes(vector<PlanNode *> &list, PlanNode *p)
{
    double maxDist = 0;
    int idx = 0;
    int ii = 0;
    while(idx < list.size())
    {
        ii = idx;
        for(int i=idx; i< list.size(); i++)
        {
            double d = sqrt((p->p-list[i]->p)*(p->p-list[i]->p));

            if(maxDist < d)
            {
                maxDist = d;
                ii = i;
            }
        }

        PlanNode * tmp = list[ii];
        list.erase(list.begin()+ii);
        list.insert(list.begin()+idx, tmp);
        idx++;
    }

}

void Drone::MoveToGoal(double step_l)
{
    if(!pathWPs.empty())
    {

//        bool tflag = false;

//        if(strategy == BREADTH_FIRST)
//        {
//            timeval seconds;
//            gettimeofday(&seconds, NULL);
//            //printf("seconds: %f /n", seconds);

//            double elapsedTime = (-last_time.tv_sec + seconds.tv_sec) * 1000.0;      // sec to ms
//            elapsedTime += (-last_time.tv_usec + seconds.tv_usec) / 1000.0;   // us to ms

//            if(elapsedTime/1000.0 < 0.2)
//               tflag = true;
//        }

        bool reachedWP = false;
        PlanNode *goalNode = pathWPs.front();

        if(wps.empty())
        {
            wps.push_back(goalNode);
        }

        if(wps.back()!= goalNode)
        {
            wps.push_back(goalNode);
        }

        //fix the observation height; one time for each node
        if(!goalNode->observationHeightFixed)
        {
            double obsHeight = World::Instance()->GetMaxHeightInRect(goalNode->p[0], goalNode->p[1], sensor.GetFootprint(goalNode->p[2]));
            if(goalNode->p[2] - obsHeight < MAX_DIST_TO_OBSTACLES)
                goalNode->p[2] = obsHeight + MAX_DIST_TO_OBSTACLES;
            goalNode->observationHeightFixed = true;
        }

        Vector<3> toWP = goalNode->p-GetPose();
//        if(tflag)
//            printf("%f %f %f %f \n",toWP[0],toWP[1],toWP[2], step_l);

        double wpd = sqrt(toWP*toWP);

        Vector<3> dv;

        if(wpd < step_l)
        {
            dv = toWP;
            reachedWP = true;
        }
        else
        {
            normalize(toWP);
            dv = (step_l)*toWP;
        }

        MoveSensor(dv);

        if(reachedWP)
        {
           // printf("at wp\n",wpd);

            //printf("hello\n");
            int curLvl = goalNode->depth;
            goalNode->visited = true;
            //delete the reached node
            pathWPs.erase(pathWPs.begin());
            //VisitWaypoint(goalNode);

            if(!goalNode->homeNode)
            {
                if(strategy == SHORTCUT_1)
                {
                    //printf("hello\n");

                    // visit the children (if any)
                    if(!goalNode->children.empty())
                    {
                        //printf("hello1\n");

                        // save the current node if it is tentative
                        if(goalNode->tentative == 1)
                        {
                            pathWPs.insert(pathWPs.begin(), goalNode);
                        }

                        //printf("Before: %d\n", goalNode->children.size());
                        SortNodes(goalNode->children, goalNode);

                        bool flag = false;
                        for(int i=0; i<goalNode->children.size() ; i++)
                        {
                            if(goalNode->children[i]->interestingness > 0.2 && !goalNode->children[i]->visited)
                            {
                                flag = true;
                                pathWPs.insert(pathWPs.begin(), goalNode->children[i]);
                            }
                        }

                        if(!flag && goalNode->tentative == 1)
                                pathWPs.erase(pathWPs.begin());
                    }
                    else
                    {
                        bool more_children = false;
                        // reached a tentative node
                        if(goalNode->tentative == 1)
                        {
                            if(tree.SubTreeInterestingLeaves(goalNode) >= 1)
                            {
                                if(!goalNode->parent->visited)
                                {
                                    goalNode->parent->visited = true;
                                    SortNodes(goalNode->parent->children, goalNode);
                                    for(int i=0; i< goalNode->parent->children.size(); i++)
                                    {
                                        if(goalNode->parent->children[i] != goalNode)
                                        {
                                            more_children = true;
                                            pathWPs.insert(pathWPs.begin(), goalNode->parent->children[i]);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                pathWPs.front()->mustVisit = true;
                                //assert(pathWPs.front() == goalNode->parent);
                            }
                        }

                        if(goalNode->tentative != 1 || !more_children)
                        {
                            if(!pathWPs.empty())
                            {
                                if(pathWPs.front()->tentative == 1 && pathWPs.front()->visited)
                                {
                                    bool flagg= false;
                                    //printf("1\n");
                                    while(!flagg && !pathWPs.empty())
                                    {
                                        PlanNode *parent =  pathWPs.front();

                                        PlanNode *grandparent = NULL;
                                        if(pathWPs.size() >=2)
                                        {
                                            grandparent = pathWPs[1];
                                        }

                                        pathWPs.erase(pathWPs.begin());

                                        //assert(parent == goalNode->parent);
                                       // printf("3\n");
                                        if(grandparent != NULL)
                                        {
                                            if(!grandparent->visited && grandparent->tentative==1 && tree.SubTreeInterestingLeaves(parent) >= 1)
                                            {
                                               // printf("4\n");

                                                grandparent->visited = true;
                                                for(int i=0; i< grandparent->children.size(); i++)
                                                {
                                                    if(grandparent->children[i] != parent && !grandparent->children[i]->visited)
                                                    {
                                                        pathWPs.insert(pathWPs.begin(), grandparent->children[i]);
                                                        flagg = true;
                                                    }
                                                }
                                               // printf("5\n");

                                            }
                                            else if(!grandparent->visited && grandparent->tentative==1 && tree.SubTreeInterestingLeaves(parent) < 1)
                                            {
                                                //printf("6\n");

                                                grandparent->mustVisit = true;
                                                flagg = true;
                                                //printf("7\n");


                                            }
                                            else
                                            {
                                                //printf("8\n");

                                                if(grandparent->tentative != 1)
                                                {
                                                    flagg = true;
                                                }

                                                //printf("hello!! %d %d %d\n", grandparent->visited, grandparent->tentative, SubTreeInterestingLeaves(parent));
                                                //exit(0);
                                                //printf("9\n");

                                            }
                                        }
                                        else
                                        {
                                            flagg = true;
                                        }
                                    }
                                    //printf("2\n");
                                }
                            }
                        }

                        if(pathWPs.front()->depth < goalNode->depth && !pathWPs.front()->mustVisit)
                        {
                            //printf("Detected a possible shortcut %d-%d from: %f %f\n",pathWPs.front()->depth, goalNode->depth,  goalNode->p[0], goalNode->p[1]);

                            PlanNode * newTarget = pathWPs.front();
                            newTarget->tentative = 1;
                            MakeShortcutPlan(goalNode, newTarget);
                        }

                    }



                }
                else if(strategy == DEPTH_FIRST)
                {
                    for(int i=0; i< goalNode->children.size(); i++)
                    {
                        if(goalNode->children[i]->interestingness > 0.2)
                            pathWPs.insert(pathWPs.begin(), goalNode->children[i]);
                    }
                }
                else if(strategy == BREADTH_FIRST)
                {
                    //printf("here 1\n");
                    //goto next depth
                    if(pathWPs.size() == 0 && tree.levelNodes.find(curLvl+1) != tree.levelNodes.end())
                    {
                        PlanForLevel(curLvl+1);
                    }
                }

                if(pathWPs.size() == 0) //go back to home position
                {
                    //printf("go back to home position\n");

                    PlanNode * home = CreatePlanNode();
                    home->p = homePos;
                    home->homeNode = true;
                    home->depth = curLvl;
                    pathWPs.push_back(home);

                    sensor.TurnOnSensing(false);
                    pathWPs[0]->p[2] = GetPose()[2];
                }

            }
            //printf("hi\n");
        }
    }
    else
    {
        char sc=0;
        if(strategy == BREADTH_FIRST)
            sc = 'B';
        else if(strategy == DEPTH_FIRST)
            sc = 'D';
        else if(strategy == SHORTCUT_1)
            sc = 'S';
        else if(strategy == LAWNMOWER)
            sc = 'L';
        else
            sc = '-';

        printf("%c %d %d %d %f\n", sc, strategy, World::Instance()->GetPercOfIntCells(), World::Instance()->GetNumOfIntCells(), surveyLength);
        //exit(0);
        strategy = (Traverse_Strategy)((char)strategy+1);
        if(strategy == NONE)
        {
            exit(0);
        }

        GoToLevel(startLevel);
        GeneratePlan();
        ExecutePlan();
//        idle = false;
//        idle = true;
    }
}

void Drone::SetExpandable(PlanNode *pn)
{
    //printf("here1\n");
    double dh = pn->p[2] - World::Instance()->GetMaxHeightInRect(pn->p[0], pn->p[1], sensor.GetFootprint(pn->p[2]));
    //printf("here3\n");

    if(dh >  MAX_DIST_TO_OBSTACLES)
    {
       // printf("dh ====  %f\n", dh);
        pn->expandable = true;
    }
    else
        pn->expandable = false;

}

PlanNode * Drone::FindClosestChild(PlanNode *parent, PlanNode *pn)
{
    if(parent->depth >= pn->depth || parent->children.empty())
    {
        //printf("no close children!!\n");
        return NULL;
    }
    else
    {
        int idx = -1;
        double minDist = 99999999999;
        for(int i=0; i<parent->children.size(); i++)
        {
            Vector<3> dp = parent->children[i]->p - pn->p;
            dp[2] = 0;

            if(dp*dp < minDist)
            {
                idx = i;
                minDist = dp*dp;
            }
        }

        return  parent->children[idx];
    }
//        int idx = -1;
//        double minDist = 99999999999;
//        for(int i=0; i<parent->children.size(); i++)
//        {
//            Vector<3> dp = parent->children[i]->p - pn->p;
//            dp[2] = 0;

//            if(dp*dp < minDist)
//            {
//                idx = i;
//                minDist = dp*dp;
//            }
//        }

//        if(parent->depth+1 == pn->depth)
//        {
//            return  parent->children[idx];
//        }
//        else
//            return FindClosestChild(parent->children[idx],pn);
//    }
}

void Drone::MakeShortcutPlan(PlanNode *bottomNode, PlanNode *topNode)
{
    PlanNode* p = FindClosestChild(topNode, bottomNode);
    assert(p!=NULL);

    while(p != NULL)
    {
       // printf("next close children!!\n");

        p->tentative = 1;
        pathWPs.insert(pathWPs.begin(), p);
        p = FindClosestChild(p, bottomNode);
    }
}

void Drone::GenerateSubtree(PlanNode *node, int tree_depth)
{
    if(!node->expandable)
        return;

    //printf("Generating subtree: from level %d for depth %d\n", node->depth, tree_depth);
    PlanNode *tmp;

    int child_n = branching_deg;
    for(int i=1; i<tree_depth; i++)
        child_n *= branching_deg;

    double footPrint_l = sensor.GetFootprint(node->p[2]);
    double next_fp = footPrint_l/child_n;
    double xx = node->p[0]-footPrint_l/2.0 + next_fp/2.0;
    double yy = node->p[1]-footPrint_l/2.0 + next_fp/2.0;
    double zz = sensor.GetHeightWithGootprint(next_fp);

  //  printf("1\n");
    for(int i=0; i<child_n; i++)
        for(int j=0; j<child_n; j++)
        {
            tmp = CreatePlanNode();
            tmp->tentative = -1;
            tmp->p = makeVector(xx + i*next_fp, yy + j*next_fp, zz);
            double dh = tmp->p[2] - World::Instance()->GetMaxHeightInRect(tmp->p[0], tmp->p[1], sensor.GetFootprint(tmp->p[2]));
            if(dh <  MAX_DIST_TO_OBSTACLES)
            {
               // printf("dh ====  %f\n", dh);
                continue;
            }

            double footprint_l = sensor.GetFootprint(tmp->p[2]);
            Vector<2, double > tl = makeVector(tmp->p[0]-footprint_l/2, tmp->p[1]+footprint_l/2);
            Vector<2, double > rb = makeVector(tmp->p[0]+footprint_l/2, tmp->p[1]-footprint_l/2);
            tmp->interestingness = sensor.GetInterestingness(tl,rb)/sensor.GetMaxInterestingness();

            SetExpandable(tmp);
            //if(tmp->interestingness > 0.2)
            {
//                if(strategy == BREADTH_FIRST)
//                {
//                    AddChild(node, tmp);
//                    nextPath.push_back(tmp);
//                }
//                else if(strategy == DEPTH_FIRST)
                {
                    tree.AddChild(node, tmp);
                    //tmp->depth = node->depth+tree_depth;
                    //pathWPs.insert(pathWPs.begin(),tmp);
                    //pathWPs.push_back(tmp);
                }
            }
//            else
//            {
//                delete tmp;
//            }
        }
}

void Drone::VisitWaypoint(PlanNode* node)
{
    if(!node->expandable)
        return;

    PlanNode *tmp;
    int child_n = branching_deg;
    double footPrint_l = sensor.GetFootprint(node->p[2]);
    double next_fp = footPrint_l/child_n;
    double xx = node->p[0]-footPrint_l/2.0 + next_fp/2.0;
    double yy = node->p[1]-footPrint_l/2.0 + next_fp/2.0;
    double zz = sensor.GetHeightWithGootprint(next_fp);

    int newNodes_n=0;
  //  printf("1\n");
    for(int i=0; i<child_n; i++)
        for(int j=0; j<child_n; j++)
        {
            tmp = CreatePlanNode();
            tmp->p = makeVector(xx + i*next_fp, yy + j*next_fp, zz);
            double dh = tmp->p[2] - World::Instance()->GetMaxHeightInRect(tmp->p[0], tmp->p[1], sensor.GetFootprint(tmp->p[2]));
            if(dh <  MAX_DIST_TO_OBSTACLES)
            {
               // printf("dh ====  %f\n", dh);
                continue;
            }

            double footprint_l = sensor.GetFootprint(tmp->p[2]);
            Vector<2, double > tl = makeVector(tmp->p[0]-footprint_l/2, tmp->p[1]+footprint_l/2);
            Vector<2, double > rb = makeVector(tmp->p[0]+footprint_l/2, tmp->p[1]-footprint_l/2);
            tmp->interestingness = sensor.GetInterestingness(tl,rb)/sensor.GetMaxInterestingness();

            SetExpandable(tmp);
            if(tmp->interestingness > 0.2)
            {
                if(strategy == BREADTH_FIRST)
                {
                    AddChild(node, tmp);
                    nextPath.push_back(tmp);
                }
                else if(strategy == DEPTH_FIRST || strategy == SHORTCUT_1)
                {
                    newNodes_n++;
                    AddChild(node, tmp);
                    pathWPs.insert(pathWPs.begin(),tmp);
                }
            }
            else
            {
                delete tmp;
            }
        }
}

void Drone::PlanForLevel(int depth)
{
    executingPlan = false;
    //printf("TSP starts\n");

    timeval scanEnd;
    gettimeofday(&scanEnd,NULL);

    double totalTime = (-scanStartTime.tv_sec + scanEnd.tv_sec);      // sec to ms
    totalTime += (-scanStartTime.tv_usec + scanEnd.tv_usec) / 1000000.0;   // us to ms

    vector<PlanNode*> &nlnds = tree.levelNodes[depth];
   // printf("hello %d %d %d\n", depth, nlnds.size(), tree.levelNodes.size());

    if(nlnds.empty())
    {
        return;
    }

    sensor.ResetInterestingness();
    curLevel++;

    tspoint.clear();

    Entity * en = new Entity();
    en->pos = GetPose();
    en->start = true;
    en->nodeIdx = -1;
    tspoint.push_back(en);

    for(int i=0; i<nlnds.size();i++)
    {
        if(nlnds[i]->interestingness > 0.2)
        {
            en = new Entity();
            en->pos = nlnds[i]->p;
            en->nodeIdx = i;
            tspoint.push_back(en);
        }
    }


    en = new Entity();
    en->pos = homePos;
    en->pos[2]=tspoint.back()->pos[2];
    en->end = true;
    en->nodeIdx = -2;

    shortestPath.clear();
    //shortestPath = tsp.GetShortestPath(tspoint);

    tsp.GetShortestPath(tspoint, shortestPath);

   // printf("TSP size: %d\n", shortestPath.size());

    for(int i=0; i<shortestPath.size();i++)
    {
        if(shortestPath[i]->nodeIdx >= 0) // other nodes
        {
            pathWPs.push_back(nlnds[shortestPath[i]->nodeIdx]);
        }
        else if(shortestPath[i]->nodeIdx == -2) // the home node
        {

//            PlanNode* pn = CreatePlanNode();
//            pn->p = homePos;
//            pn->homeNode = true;
//            double height;
//            if(!pathWPs.empty())
//            {
//                height = pathWPs.back()->p[2];
//            }
//            else
//                height = GetPose()[2];

//            pn->p[2] = height;

//            pathWPs.push_back(pn);

        }
        if(shortestPath[i]->nodeIdx == -1) // the start node (current position)
        {
//            PlanNode* pn= CreatePlanNode();
//            pn->p = GetPose();
//            pathWPs.push_back(pn);
        }
    }

   // printf("Done with TSP 1\n");

    while(!tspoint.empty())
    {
        Entity * p = tspoint.back();
        tspoint.pop_back();
        delete p;
    }

    while(!shortestPath.empty())
    {
        Entity * p = shortestPath.back();
        shortestPath.pop_back();
        //already destroyed in tspoint vector
        // delete p;
    }

    //printf("Done with TSP\n");
    executingPlan = true;

    gettimeofday(&last_time, NULL);

    //tspoint.clear();

//    while(!nextPath.empty())
//    {
//        PlanNode * p = nextPath.back();
//        nextPath.pop_back();
//        // nodes will be destroyed when visited
//        // delete p;
//    }

//    for(int i=0; i<nextPath.size(); i++)
//    {
//        delete nextPath[i];
//    }
    //nextPath.clear();

   // printf("#POI: %d\n",pathWPs.size());
}
