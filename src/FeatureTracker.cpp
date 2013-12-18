#include "FeatureTracker.h"
#include "GL/gl.h"
#include "World.h"

#define RAND(a,b)    a+(b-a)*((double)(rand()%1000))/1000

FeatureTracker::FeatureTracker(TooN::Vector<3,double> startPos):Visualizer()
{
    pose = startPos;

    for(int i=0; i< 1000; i++)
    {
        Feature f;
        f.pos[0] = RAND(-5,5);
        f.pos[1] = RAND(-5,5);

        f.pos[2] = 0.01 + World::Instance()->GetHeight(f.pos[0], f.pos[1]);

        features.push_back(f);
    }
}

void FeatureTracker::glDraw()
{

    glColor3f(1,1,1);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex3f(pose[0], pose[1], pose[2]);
    glEnd();

    double dxy = pose[2] * tan(FOV/2);

    glColor3f(0,0,0);
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex3f(pose[0], pose[1], pose[2]);
    glVertex3f(pose[0], pose[1], pose[2]-0.5);

    glVertex3f(pose[0], pose[1], pose[2]);
    glVertex3f(pose[0]+dxy, pose[1]+dxy, 0);

    glVertex3f(pose[0], pose[1], pose[2]);
    glVertex3f(pose[0]+dxy, pose[1]-dxy, 0);

    glVertex3f(pose[0], pose[1], pose[2]);
    glVertex3f(pose[0]-dxy, pose[1]+dxy, 0);

    glVertex3f(pose[0], pose[1], pose[2]);
    glVertex3f(pose[0]-dxy, pose[1]-dxy, 0);
    glEnd();


    std::vector<Feature> fs = MatchedFeatures(pose);

    glColor3f(1,0,0);
    glPointSize(5);
    glBegin(GL_POINTS);

    for(int i=0; i<fs.size(); i++)
        glVertex3f(fs[i].pos[0], fs[i].pos[1], fs[i].pos[2]);

    glEnd();


}

void FeatureTracker::MoveSensor(TooN::Vector<3, double> dPos)
{
    pose += dPos;
}

bool FeatureTracker::InsideFOV(Feature f, Vector<3, double> pos)
{
    return fabs(f.pos[0]-pos[0]) < fabs(pos[2]*tan(FOV/2)) && fabs(f.pos[1]-pos[1]) < fabs(pos[2]*tan(FOV/2));
}

std::vector<Feature> FeatureTracker::MatchedFeatures(Vector<3, double> viewpoint)
{
    vector<Feature> result;

    for(int i=0; i< features.size(); i++)
    {
        if(InsideFOV(features[i], viewpoint))
            result.push_back(features[i]);
    }

    return result;
}
