#ifndef DRONE_
#define DRONE_

#include "Visualizer.h"
#include "FeatureTracker.h"
#include "TooN/TooN.h"
#include "World.h"
#include "TSP.h"

#define     MAX_DIST_TO_OBSTACLES   1.5

//std::string strategy_name_str(int idx)
//{
//    if(idx==0)
//        return "BreadthFirst";
//    else if(idx==1)
//        return "DepthFirst";
//    else if(idx==2)
//        return "Shortcut";
//    else if(idx==3)
//        return "Lawnmower";
//    else
//        return "none";
//}

using namespace TooN;
using namespace std;
//class Drone;
//int Drone::SubTreeInterestingLeaves(PlanNode* root);
struct PlanNode
{
    Vector<3> p;
    double interestingness;
    vector<PlanNode*> children;
    PlanNode* parent;

    bool visited;
    bool expandable;
    bool homeNode;

    bool mustVisit;
    int depth;
    int tentative;
    bool observationHeightFixed;

};


struct Tree
{
    PlanNode * root;
    map<int, vector<PlanNode*> > levelNodes;

    void CreateTree()
    {
        root = new PlanNode();
        root->tentative = -1;
        root->parent = NULL;
        root->depth = 0;
        root->visited = true;
    }

    int SubTreeInterestingLeaves(PlanNode *r)
    {
        if(r->children.empty())
        {
            if(r->interestingness > 0.2)
                return 1;
            else
                return 0;
        }
        else
        {
            int n = 0;
            for(int i=0; i<r->children.size(); i++)
            {
                n += SubTreeInterestingLeaves(r->children[i]);
            }

            return n;
        }
    }

    void glDraw()
    {
        glDisable(GL_TEXTURE_2D);
        glLineStipple(1, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);

        for(int i=0; i<levelNodes.size(); i++)
        {
            vector<PlanNode*> &nds = levelNodes[i];
            glColor3f(0,0,1-(1.0*i)/levelNodes.size());
            //glLineWidth(levelNodes.size()-0.5*i);
            glLineWidth(2);
            glBegin(GL_LINES);
            for(int j=0; j<nds.size(); j++)
            {
//               / if(SubTreeInterestingLeaves(nds[j]) > 0)
                {
                    glVertex3f(nds[j]->p[0],nds[j]->p[1],nds[j]->p[2]);
                    glVertex3f(nds[j]->parent->p[0],nds[j]->parent->p[1],nds[j]->parent->p[2]);
                }
            }
            glEnd();

            glColor3f(0,0,0);
            glBegin(GL_POINTS);
            glPointSize(10);
            for(int j=0; j<nds.size(); j++)
            {
//               / if(SubTreeInterestingLeaves(nds[j]) > 0)
                {
                    glVertex3f(nds[j]->p[0],nds[j]->p[1],nds[j]->p[2]);

                }
            }
            glEnd();

        }
        glDisable(GL_LINE_STIPPLE);



    }

    void AddChild(PlanNode* parent, PlanNode* node)
    {
        node->depth = parent->depth+1;
        node->parent = parent;
        parent->children.push_back(node);

        if(levelNodes.find(node->depth) != levelNodes.end())
        {
            levelNodes[node->depth].push_back(node);
        }
        else
        {
            vector<PlanNode*> lvl;
            lvl.push_back(node);
            levelNodes.insert(pair<int, vector<PlanNode*> >(node->depth, lvl));
        }

    }

    void Destroy()
    {
        levelNodes.clear();
        vector<PlanNode*> nds;
        DepthFirst(root, nds);
        for(int i=0; i<nds.size();i++)
        {
            PlanNode * nd = nds.back();
            nds.pop_back();
            delete nd;
        }

    }

    void DepthFirst(PlanNode* root, vector<PlanNode*> &nodes)
    {
        if(root->children.empty())
        {
            nodes.push_back(root);
            return;
        }
        else
        {
            for(int i=0; i<root->children.size(); i++)
            {
                DepthFirst(root->children[i], nodes);
            }
        }
    }
};


class Drone:public Visualizer
{
public:
    enum Traverse_Strategy {DEPTH_FIRST=0, SHORTCUT_1, BREADTH_FIRST,LAWNMOWER, NONE};
    Drone();
    ~Drone();

    void glDraw();
    void Update();

    void init(int branchingDeg, int startlvl, Traverse_Strategy st);

    void MoveSensor(TooN::Vector<3,double> dPos);
    Vector<3> GetPose();
    void GoLevelDown();
    void GoLevelUp();
    void GoToLevel(int lvl);
    void ToggleStealthMode();
    void ToggleDrawInterestingness();

    void GeneratePlan();
    void ExecutePlan();
    void DestroyPlan();

    void MoveToGoal(double step_l);
    void ChangeSpeed(double ds);

    static PlanNode * CreatePlanNode();
    double speed;

private:
    PlanNode * FindClosestChild(PlanNode* parent, PlanNode* pn);
    void MakeShortcutPlan(PlanNode *bottomNode, PlanNode *topNode);
    void GenerateSubtree(PlanNode * root, int tree_depth);
    void SetExpandable(PlanNode *pn);
    void VisitWaypoint(PlanNode *node);
    void GenerateCoveragePlan(double w_w, double w_l, double flying_height);
    void PlanForLevel(int depth);
    double PathLength(vector< PlanNode* > & path);
    void AddChild(PlanNode* parent, PlanNode* child);
    void DrawCell(PlanNode *p);
    void SortNodes(vector<PlanNode*> &list, PlanNode* p);
    FeatureTracker sensor;

    timeval last_time;

    bool idle;
    bool newPlan;
    bool executingPlan;
    int levels;
    int startLevel;
    double footprint_length;
    int curLevel;
    timeval scanStartTime;
    int branching_deg;
    Traverse_Strategy strategy;

    Tree tree;
    //executed path
    vector< PlanNode* > pathWPs;
    vector< PlanNode* > nextPath;
    vector< PlanNode* > wps;

    vector<Entity*> tspoint;

    Vector<3> homePos;
    vector<Entity*> shortestPath;
    TSP tsp;
    double surveyLength;

};


#endif
