#include "FeatureTracker.h"
#include "GL/gl.h"
#include "World.h"
#include <stdio.h>
//#define RAND(a,b)    a+((double)(b-a))*((double)(rand()%1000))/1000
//#define RAND(a,b)    (a)+((b)-(a))*((double)(rand()%1000))/1000
#define MIN_FEATURES    30
#define MAX(a,b)    (a>b)?a:b


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

//    vector<Entity*> ev;
//    for(int i=0;i<10; i++)
//    {
//        double ww = 5;
//        double wl = 5;
//        Vector<3> p = makeVector(pow(-1,i)*i,i,4);//*/makeVector(RAND(-ww/2,ww/2), RAND(-wl/2,wl/2), 4);
//        Entity * en = new Entity();
//        en->pos = p;
//        if(i==0)
//            en->start = true;
//        else
//            en->start = false;

//        ev.push_back(en);
//    }

//    Vector<3> p = makeVector(101,1,4);//*/makeVector(RAND(-ww/2,ww/2), RAND(-wl/2,wl/2), 4);
//    Entity * en1 = new Entity();
//    en1->pos = p;
//    en1->start = false;
//    ev.push_back(en1);

//    p = makeVector(100,1,4);//*/makeVector(RAND(-ww/2,ww/2), RAND(-wl/2,wl/2), 4);
//    Entity * en2 = new Entity();
//    en2->pos = p;
//    en2->start = false;
//    ev.push_back(en2);

   // shortestPath = tsp.GetShortestPath(ev);

}

void FeatureTracker::DrawFrustum(Vector<3, double> camp, double size)
{
    glColor3f(1,0,0);
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
//    static int lastt = 0;
//    static int idx=0;
//    lastt++;
//    if(lastt > 100)
//    {
//        lastt = 0;
//        idx = RAND(0,features.size()-1);
//    }

//    if(idx >0 && idx < features.size())
//    {
//        glLineWidth(5);
//        glColor3f(1,0,0);
//        glBegin(GL_LINES);
//        glVertex3f(features[idx].view[0]-features[idx].size,features[idx].view[1],features[idx].view[2]);
//        glVertex3f(features[idx].view[0]+features[idx].size,features[idx].view[1],features[idx].view[2]);
//        glEnd();

//        glBegin(GL_POINTS);
//        glVertex3f(features[idx].pos[0],features[idx].pos[1],features[idx].pos[2]);
//        glEnd();
//    }

    DrawFrustum(pose, pose[2]);

    //std::vector<Feature> fs = MatchedFeatures(pose);
    std::vector<Feature> fs = features;

    glColor3f(1,0,0);
    glPointSize(5);

    if(false)
    {
        for(int i=0; i<fs.size(); i++)
        {
            glPointSize(5*fs[i].size);
            glBegin(GL_POINTS);
            glVertex3f(fs[i].pos[0], fs[i].pos[1], fs[i].pos[2]+0.01);
            glEnd();
        }
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




//    if(bDrawEntropy)
//    {
//        //Draw entropy field
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        for(int i=0; i< entropies.size(); i++)
//        {
//            glColor4f(0,0,1,entropies[i]/mesh.maxEntropy);
//            Vector<3, double > p1 = makeVector(nextPath[i][0]-footprint_length/4, nextPath[i][1]+footprint_length/4, nextPath[i][2]);
//            Vector<3, double > p2 = makeVector(nextPath[i][0]+footprint_length/4, nextPath[i][1]-footprint_length/4, nextPath[i][2]);

//            glBegin(GL_POLYGON);
//            glVertex3f(p1[0], p1[1], p1[2]);
//            glVertex3f(p2[0], p1[1], p2[2]);
//            glVertex3f(p2[0], p2[1], p2[2]);
//            glVertex3f(p1[0], p2[1], p2[2]);
//            glEnd();
//        }
//    }
}

void FeatureTracker::ClearHistory()
{
    bSensing = false;
    features.clear();
    matchedFeatures.clear();
    mesh.CleanMesh();
    //nextPath.clear();
    entropies.clear();
}

void FeatureTracker::MoveSensor(TooN::Vector<3, double> dPos)
{
    SetPose(pose + dPos);
}

void FeatureTracker::MoveSensorTo(Vector<3,double> pos)
{
    SetPose(pos);
}

double FeatureTracker::GetInterestingness(TooN::Vector<2, double> tl, TooN::Vector<2, double> br)
{
    //return mesh.GetInterestingness(tl,br);
    World::Instance()->GetInterestingness(tl,br);
}

double FeatureTracker::GetFootprint(double sensorHight)
{
    return 2*tan(FOV/2)*sensorHight;
}

double FeatureTracker::GetHeightWithGootprint(double footprint_l)
{
    return 0.5*footprint_l/tan(FOV/2);
}

void FeatureTracker::SetPose(Vector<3, double> newpose)
{
    pose = newpose;

    if(bSensing)
    {
        vector<Feature> fs = TrackFeatures(pose);

        double rndMin = MIN_FEATURES * RAND(0.7,1.3);
        //printf("%d\n",fs.size());

        if(fs.size() < rndMin)
        {
            GenerateFeatures(rndMin - fs.size(), pose);
        }

        UpdateMatchedFeatures();
    }

}
double FeatureTracker::GetSamplingLevels(int &level)
{
    //return ((double)level) * 0.5;
    //double wl = World::Instance()->GetLength();
    if(level<1)
        level = 1;


    double ww = World::Instance()->GetWidth();

    double max_l = ww;//MAX(wl,ww);

    double dv=1;
    for(int i=0;i<level-1; i++)
    {
        //dv*=2;
        dv += 1;
    }
    double result = (0.5*max_l/dv)*tan(FOV/2);
    if(result < 0.1/*World::Instance()->GetMaxHeight()*/)
    {
        if(level <=1)
        {
            printf("Too high world!!");
            return 0;
        }
        else
        {
            printf("Too no lower level!! At level: %d", level);
            return GetSamplingLevels(--level);
        }
    }
    else
        return result;
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

        f.pos[2] = World::Instance()->GetHeight(f.pos[0], f.pos[1]);//+RAND(0,0.1)*viewpoint[2];
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

            Vector<3> prj2d = viewpoint-features[i].view;
            double dz = fabs(prj2d[2]);
            prj2d[2] = 0;

            double dist2d = sqrt(prj2d*prj2d);
            if(dist2d < features[i].size && dz < 2)
            {
                result.push_back(features[i]);
            }
        }
    }

    return result;
}

void FeatureTracker::OnPlanExecuted()
{
//    double footPrint_l = footprint_length/2;
//    for(int i=0; i< nextPath.size(); i++)
//    {
//        Vector<2, double > tl = makeVector(nextPath[i][0]-footPrint_l/2, nextPath[i][1]+footPrint_l/2);
//        Vector<2, double > rb = makeVector(nextPath[i][0]+footPrint_l/2, nextPath[i][1]-footPrint_l/2);
//        entropies.push_back(mesh.GetEntropy(tl,rb));
//    }
}
