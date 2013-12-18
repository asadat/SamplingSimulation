#include "FeatureTracker.h"
#include "GL/gl.h"
#include "World.h"

#include <stdio.h>

//#define RAND(a,b)    a+((double)(b-a))*((double)(rand()%1000))/1000
#define MIN_FEATURES    10

FeatureTracker::FeatureTracker(TooN::Vector<3,double> startPos):Visualizer()
{
    srand(time(NULL));
    SetPose(startPos);

//    for(int i=0; i< 1000; i++)
//    {
//        Feature f;
//        f.pos[0] = RAND(-5,5);
//        f.pos[1] = RAND(-5,5);

//        f.pos[2] = 0.01 + World::Instance()->GetHeight(f.pos[0], f.pos[1]);

//        features.push_back(f);
//    }

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


    //std::vector<Feature> fs = MatchedFeatures(pose);
    std::vector<Feature> fs = features;

    glColor3f(1,0,0);
    glPointSize(5);


    for(int i=0; i<fs.size(); i++)
    {
        glPointSize(10*fs[i].size);
        glBegin(GL_POINTS);
        glVertex3f(fs[i].pos[0], fs[i].pos[1], fs[i].pos[2]+0.01);
        glEnd();
    }
}

void FeatureTracker::MoveSensor(TooN::Vector<3, double> dPos)
{
    SetPose(pose + dPos);
}

void FeatureTracker::SetPose(Vector<3, double> newpose)
{
    pose = newpose;

    vector<Feature> fs = MatchedFeatures(pose);
    if(fs.size() < MIN_FEATURES)
    {
        GenerateFeatures(MIN_FEATURES - fs.size(), pose);
    }
}

void FeatureTracker::GenerateFeatures(int size, Vector<3, double> viewpoint)
{

    for(int i=0; i< size; i++)
    {
        Feature f;
        f.view = viewpoint;

        double x1 = viewpoint[0]-viewpoint[2]*tan(FOV/2);
        double x2 = viewpoint[0]+viewpoint[2]*tan(FOV/2);

        double y1 = viewpoint[1]-viewpoint[2]*tan(FOV/2);
        double y2 = viewpoint[1]+viewpoint[2]*tan(FOV/2);


        f.pos[0] = RAND(x1, x2);
        f.pos[1] = RAND(y1, y2);

        f.pos[2] = World::Instance()->GetHeight(f.pos[0], f.pos[1]);
        f.size = (viewpoint[2] - f.pos[2])*tan(FOV/2)*RAND(0.5, 0.8);

        //printf("%f %f %f %f\n", f.pos[0], f.pos[1], f.pos[2], f.size);

        features.push_back(f);
    }

//    printf("Generating Features %d\n", features.size());
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
        {
            double dist = sqrt((viewpoint-features[i].view)*(viewpoint-features[i].view));
            if(dist < features[i].size)
            {
                result.push_back(features[i]);
            }
        }
    }

   // printf("Generating Features %d\n", result.size());

    return result;
}
