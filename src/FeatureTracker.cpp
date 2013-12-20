#include "FeatureTracker.h"
#include "GL/gl.h"
#include "World.h"

#include <stdio.h>

//#define RAND(a,b)    a+((double)(b-a))*((double)(rand()%1000))/1000
#define MIN_FEATURES    50

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

void FeatureTracker::DrawFrustum(Vector<3, double> camp, double size)
{
    glColor3f(1,1,1);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex3f(camp[0], camp[1], camp[2]);
    glEnd();

    double dxy = size * tan(FOV/2);

    glColor3f(0,0,0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(camp[0], camp[1], camp[2]);
    glVertex3f(camp[0], camp[1], camp[2]-0.5);

    glVertex3f(camp[0], camp[1], camp[2]);
    glVertex3f(camp[0]+dxy, camp[1]+dxy, camp[2]-size);

    glVertex3f(camp[0], camp[1], camp[2]);
    glVertex3f(camp[0]+dxy, camp[1]-dxy, camp[2]-size);

    glVertex3f(camp[0], camp[1], camp[2]);
    glVertex3f(camp[0]-dxy, camp[1]+dxy, camp[2]-size);

    glVertex3f(camp[0], camp[1], camp[2]);
    glVertex3f(camp[0]-dxy, camp[1]-dxy, camp[2]-size);
    glEnd();


}

void FeatureTracker::glDraw()
{
    DrawFrustum(pose, pose[2]);

    //std::vector<Feature> fs = MatchedFeatures(pose);
    std::vector<Feature> fs = features;

    glColor3f(1,0,0);
    glPointSize(5);


    for(int i=0; i<fs.size(); i++)
    {
        glPointSize(5*fs[i].size);
        glBegin(GL_POINTS);
        glVertex3f(fs[i].pos[0], fs[i].pos[1], fs[i].pos[2]+0.01);
        glEnd();
    }

    // draw matched features
//    glColor3f(1,0,1);
//    for(int i=0; i<matchedFeatures.size(); i++)
//    {
//        glPointSize(10*matchedFeatures[i].size);
//        glBegin(GL_POINTS);
//        glVertex3f(matchedFeatures[i].pos[0], matchedFeatures[i].pos[1], matchedFeatures[i].pos[2]+0.01);
//        glEnd();
//    }


    // draw path
    for(int i=0; i<pathWPs.size(); i++)
    {
        DrawFrustum(pathWPs[i], pathWPs[i][2]);

        if(i+1<pathWPs.size())
        {
            glColor3f(0,0,0);
            glLineWidth(3);
            glBegin(GL_LINES);
            glVertex3f(pathWPs[i][0],pathWPs[i][1],pathWPs[i][2]);
            glVertex3f(pathWPs[i+1][0],pathWPs[i+1][1],pathWPs[i+1][2]);
            glEnd();
        }
    }

    if(bDrawEntropy)
    {
        //Draw entropy field
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for(int i=0; i< entropies.size(); i++)
        {
            glColor4f(0,0,1,entropies[i]/mesh.maxEntropy);
            Vector<3, double > p1 = makeVector(nextPath[i][0]-footprint_length/4, nextPath[i][1]+footprint_length/4, nextPath[i][2]);
            Vector<3, double > p2 = makeVector(nextPath[i][0]+footprint_length/4, nextPath[i][1]-footprint_length/4, nextPath[i][2]);

            glBegin(GL_POLYGON);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p1[1], p2[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p1[0], p2[1], p2[2]);
            glEnd();
        }
    }
}

void FeatureTracker::ClearHistory()
{
    bSensing = false;
    features.clear();
    matchedFeatures.clear();
    mesh.CleanMesh();
    nextPath.clear();
    entropies.clear();
}

void FeatureTracker::MoveSensor(TooN::Vector<3, double> dPos)
{
    SetPose(pose + dPos);
}

void FeatureTracker::SetPose(Vector<3, double> newpose)
{
    pose = newpose;

    if(bSensing)
    {
        vector<Feature> fs = TrackFeatures(pose);
        double rndMin = MIN_FEATURES * RAND(0.7,1.3);
        if(fs.size() < rndMin)
        {
            GenerateFeatures(rndMin - fs.size(), pose);
        }

        UpdateMatchedFeatures();
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

        mesh.AddVertex(f.pos);

        features.push_back(f);
    }
}

bool FeatureTracker::InsideFOV(Feature f, Vector<3, double> pos)
{
    return fabs(f.pos[0]-pos[0]) < fabs(pos[2]*tan(FOV/2)) && fabs(f.pos[1]-pos[1]) < fabs(pos[2]*tan(FOV/2));
}

void FeatureTracker::UpdateMatchedFeatures()
{
    matchedFeatures = TrackFeatures(pose);
}

std::vector<Feature> FeatureTracker::TrackFeatures(Vector<3, double> viewpoint)
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

    return result;
}

void FeatureTracker::ExecuteCoveragePlan(double w_w, double w_l, double flying_height, double step_l)
{
    bSensing = false;

    double footPrint_l = 2*flying_height*tan(FOV/2);
    footprint_length = footPrint_l;

    Vector<3, double> startPoint = makeVector(-w_w/2, -w_l/2, flying_height);
    SetPose(startPoint+ makeVector(footPrint_l/2,footPrint_l/2,0));

    pathWPs.clear();
    //printf("start point: %f %f %f", pose[0], pose[1], pose[2]);

    bool alternateFlag = false;
    for(int i=0; i< ceil(w_w/footPrint_l); i++)
    {
        double x = startPoint[0] + footPrint_l/2 + ((double)i)*footPrint_l;

        alternateFlag = !alternateFlag;

        for(int j=(alternateFlag)?0:(ceil(w_l/footPrint_l)-1); (alternateFlag)?(j<ceil(w_l/footPrint_l)):(j>=0);(alternateFlag)?(j++):(j--))
        {
            double y = startPoint[1] + footPrint_l/2 + ((double)j)*footPrint_l;

            pathWPs.push_back(makeVector(x,y, flying_height));

            nextPath.push_back(makeVector(x+footPrint_l/4,y+footPrint_l/4, flying_height));
            nextPath.push_back(makeVector(x+footPrint_l/4,y-footPrint_l/4, flying_height));
            nextPath.push_back(makeVector(x-footPrint_l/4,y+footPrint_l/4, flying_height));
            nextPath.push_back(makeVector(x-footPrint_l/4,y-footPrint_l/4, flying_height));
        }

    }

}

void FeatureTracker::GoToNextWP(double step_l)
{
    bSensing = true;
    for(int i=0; i+1 < pathWPs.size(); i++)
    {
        double wpd = sqrt((pathWPs[i+1]-pathWPs[i])*(pathWPs[i+1]-pathWPs[i]));
        for(int j =0; j< ceil(wpd/step_l); j++)
        {
            //SetPose(/*pose + (step_l/wpd)*(pathWPs[i+1]-pathWPs[i])*/pathWPs[i]);
            MoveSensor((step_l/wpd)*(pathWPs[i+1]-pathWPs[i]));
        }

        if(pathWPs.size() == 2)
        {
            OnPlanExecuted();
            pathWPs.clear();
        }
        else
        {
            pathWPs.erase(pathWPs.begin());
        }

        break;
    }
}

void FeatureTracker::OnPlanExecuted()
{
    double footPrint_l = footprint_length/2;
    for(int i=0; i< nextPath.size(); i++)
    {
        Vector<2, double > tl = makeVector(nextPath[i][0]-footPrint_l/2, nextPath[i][1]+footPrint_l/2);
        Vector<2, double > rb = makeVector(nextPath[i][0]+footPrint_l/2, nextPath[i][1]-footPrint_l/2);
        entropies.push_back(mesh.GetEntropy(tl,rb));
    }
}
