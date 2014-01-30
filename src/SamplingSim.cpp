#include "SamplingSim.h"
#include "GL/glut.h"
#include "TooN/TooN.h"
#include <map>
#include "World.h"
//#include "FeatureTracker.h"
#include <sys/time.h>

using namespace TooN;

SamplingSim * SamplingSim::instance = NULL;
bool visualize = true;

unsigned int updateMS = 33;
bool doUpdate = false;
bool ScanRunning = false;
timeval last_time;
bool firstFrame = true;

std::map<unsigned char, bool> Key;
bool Mouse_Left, Mouse_Right, Mouse_Middle;
TooN::Vector<2, int> Mouse_Position;
TooN::Vector<2, int> Mouse_Change;

// track camera position
TooN::Vector<3> Cam_Target;
TooN::Vector<3> Cam_Rotation;

void translateCamera(double x, double y, double z)
{
    doUpdate = true;
    Cam_Target[0] += cos(Cam_Rotation[0])*y;
    Cam_Target[1] += sin(Cam_Rotation[0])*y;
    Cam_Target[0] += cos(Cam_Rotation[0]+M_PI/2)*cos(Cam_Rotation[1])*x;
    Cam_Target[1] += sin(Cam_Rotation[0]+M_PI/2)*cos(Cam_Rotation[1])*x;
    Cam_Target[2] += sin(Cam_Rotation[1])*x + z;
}

void rotateCamera(double yaw, double pitch, double roll)
{
    doUpdate = true;
    Cam_Rotation[0] += yaw;
    Cam_Rotation[1] += pitch;
    Cam_Rotation[2] += roll;
}

void update_event(int ms)
{
   // static FeatureTracker featureTracker(makeVector(0,0,3));

    static bool firsttime =true;

    if(firsttime)
    {

        //featureTracker.ExecuteCoveragePlan(World::Instance()->GetWorldWidth(), World::Instance()->GetWorldLength(), 2, 0.5);
        firsttime = false;
    }


    doUpdate = false; // any change and we go again. If we don't change anything, we stop updating

    SamplingSim::Instance()->hanldeKeyPressed(Key, doUpdate);
    // actions:
    // - W or Left_Mouse = Move Forward constant rate
    // - S or Middle_Mouse = Move Backward constant rate
    // - A and Right_Mouse = Strafe Left constant rate
    // - D and Right_Mouse = Strafe Right constant rate
    // - A = Rotate Left constant rate
    // - D = Rotate Right constant rate
    // - R = Ascend constant rate
    // - F = Descend constant rate
    // - Right Click = Yaw + Pitch camera variable rate

    double moveRate = (static_cast<double>(ms) / 1000.0) * 20.;
    double angleRate = (static_cast<double>(ms) / 1000.0) * 1.;
    if(Key['w'] || Mouse_Left) // move forward
        translateCamera(moveRate, 0, 0);

    if(Key['s'] || Mouse_Middle) // move backward
        translateCamera(-moveRate, 0, 0);

    if(Key['a'] && Mouse_Right) // strafe left
        translateCamera(0, -moveRate, 0);

    if(Key['d'] && Mouse_Right) // strafe right
        translateCamera(0, moveRate, 0);

    if(Key['a'] && !Mouse_Right) // yaw left
        rotateCamera(angleRate, 0, 0);

    if(Key['d'] && !Mouse_Right) // yaw right
        rotateCamera(-angleRate, 0, 0);

    if(Key['r']) // Ascend
        translateCamera(0, 0, moveRate);

    if(Key['f']) // Descend
        translateCamera(0, 0, -moveRate);




    if(Mouse_Right)
    {
        // yaw and pitch camera
        double rate = 0.01;
        rotateCamera(Mouse_Change[0] * rate, Mouse_Change[1] * rate, 0);
        Mouse_Change = makeVector(0,0);
    }

    glutPostRedisplay();

    if(doUpdate)
        glutTimerFunc(ms, update_event, ms);
}

void idle_event()
{
    SamplingSim::Instance()->idle();
    //if(!ros::ok())
    // exit(0);
}



void render_event()
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // reset camera
    glLoadIdentity();

    double cosa = cos(Cam_Rotation[0]);
    double cosb = cos(Cam_Rotation[2]);
    double cosc = cos(Cam_Rotation[1]);
    double sina = sin(Cam_Rotation[0]);
    double sinb = sin(Cam_Rotation[2]);
    double sinc = sin(Cam_Rotation[1]);

    Matrix<3> rotMatrix = Data(
                cosa*cosb, cosa*sinb*sinc-sina*cosc, cosa*sinb*cosc+sina*sinc,
                sina*cosb, sina*sinb*sinc+cosa*cosc, sina*sinb*cosc-cosa*sinc,
                -sinb, cosb*sinc, cosb*cosc);

    Vector<3> Cam_Plane = rotMatrix * makeVector(0,1,0);
    Vector<3> Cam_Normal = rotMatrix * makeVector(0,0,1);

    Cam_Plane += Cam_Target;

    gluLookAt(Cam_Target[0], Cam_Target[1], Cam_Target[2],
            Cam_Plane[0], Cam_Plane[1], Cam_Plane[2],
            Cam_Normal[0], Cam_Normal[1], Cam_Normal[2]);



   // World::Instance()->glDraw();
    if(visualize)
        Visualizer::glDrawAll();


    glutSwapBuffers();
}


void resize_event(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;
    float ratio = static_cast<float>(w) / static_cast<float>(h);

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    // clipping plane = 1 to 1000
    gluPerspective(60,ratio,0.1,1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void mouse_drag_event(int x, int y)
{
    static bool ignore = false;
    if(ignore)
    {
        ignore = false;
    }else if(Mouse_Right)
    {
        Mouse_Change += (Mouse_Position - makeVector(x, y));
        //Mouse_Position = makeVector(x, y);
        //glutWarpPointer(Mouse_Position[0], Mouse_Position[1]);
        if(!doUpdate)
            update_event(updateMS);
        ignore = true;
        glutWarpPointer(Mouse_Position[0], Mouse_Position[1]);
    }else
    {
        Mouse_Position = makeVector(x, y);
        //Mouse_Change = makeVector(0 ,0);
    }
}

void mouse_move_event(int x, int y)
{
    Mouse_Position = makeVector(x, y);
}

void mouse_event(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        Mouse_Left = (state == GLUT_DOWN);
    }else if(button == GLUT_RIGHT_BUTTON)
    {
        Mouse_Right = (state == GLUT_DOWN);
        if(Mouse_Right)
        {
            glutSetCursor(GLUT_CURSOR_NONE);
        }else
        {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }else if(button == GLUT_MIDDLE_BUTTON)
    {
        Mouse_Middle = (state == GLUT_DOWN);
    }
    if(!doUpdate)
        update_event(updateMS);
}

void keyboard_event(unsigned char key, int x, int y)
{
    Key[key] = true;
    if(!doUpdate)
        update_event(updateMS);
}

void keyboard_up_event(unsigned char key, int x, int y)
{
    Key[key] = false;
}

SamplingSim::SamplingSim(int argc, char **argv)
{
    int bdeg=2;
    int speed=2000;
    int int_cells=2;
    int startlvl=2;
    int interesting_per = 20;
    Drone::Traverse_Strategy strategy = Drone::BREADTH_FIRST;

    for(int i=1; i<argc;i++)
    {
        if(strcmp(argv[i],"-i")==0)
        {
            interesting_per = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-b")==0)
        {
            bdeg = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-s")==0)
        {
            speed = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-n")==0)
        {
            int_cells = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-l")==0)
        {
            startlvl = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-nov")==0)
        {
            visualize = false;
        }
        else if(strcmp(argv[i],"-breadth")==0)
        {
            strategy = Drone::BREADTH_FIRST;
        }
        else if(strcmp(argv[i],"-depth")==0)
        {
            strategy = Drone::DEPTH_FIRST;
        }
        else if(strcmp(argv[i],"-shortcut")==0)
        {
            strategy = Drone::SHORTCUT_1;
        }
        else if(strcmp(argv[i],"-lawnmower")==0)
        {
            strategy = Drone::LAWNMOWER;
        }
    }

    world = World::Instance();
    glutInit(&argc, argv);

    world->PopulateWorld(interesting_per, int_cells);

    drone.speed = speed;
    drone.init(bdeg, startlvl, strategy);
}

SamplingSim::~SamplingSim()
{

}

void SamplingSim::hanldeKeyPressed(std::map<unsigned char, bool> &key, bool &updateKey)
{
    updateKey = true;

    if(key['`'])
    {
        world->PopulateWorld(20);
    }

    if(key[']'])
    {
        drone.ChangeSpeed(2);
    }

    if(key['['])
    {
        drone.ChangeSpeed(-2);
    }

    if(key['u'])
    {
        //SamplingSim::Instance()->drone.sensor
        drone.MoveSensor(makeVector(0,0.1,0));
    }

    if(key['j'])
    {
        drone.MoveSensor(makeVector(0,-0.1,0));
    }

    if(key['h'])
    {
        drone.MoveSensor(makeVector(-0.1,0,0));
    }

    if(key['k'])
    {
        drone.MoveSensor(makeVector(0.1,0,0));
    }

   // static int levels=1;
    if(key['l'])
    {
        drone.GoLevelDown();
        updateKey = false;
    }

    if(key['o'])
    {
        drone.GoLevelUp();
        updateKey = false;
    }

    if(key['1'])
    {
        world->ToggleDraw();
        updateKey = false;
    }

    if(key['2'])
    {
        drone.ToggleStealthMode();
        //drone.sensor.ToggleSensing();
        updateKey = false;
    }

    if(key['3'])
    {
        drone.ToggleDrawInterestingness();
        //drone.sensor.ToggleDrawEntropyField();
        updateKey = false;
    }

    //static bool generatePlan = true;
    if(key['='])
    {
        drone.ExecutePlan();

//        if(generatePlan)
//        {
//            generatePlan = false;

//        }

//        ScanRunning=true;
//        //featureTracker.GoToNextWP(0.25);


//        if(ScanRunning)
//        {



//        }
    }

    if(key['-'])
    {
        drone.DestroyPlan();
//        firstFrame = true;
//        generatePlan = true;

    }

    if(key['0'])
    {
        drone.GeneratePlan();
    }
}

void SamplingSim::idle()
{
    drone.Update();
}

void SamplingSim::mainLoop()
{

    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("SamplingSim");
    glClearColor(1,1,1,0);
    glEnable(GL_POINT_SMOOTH);

    // register callbacks
    glutDisplayFunc(render_event);
    glutReshapeFunc(resize_event);
    glutKeyboardFunc(keyboard_event);
    glutKeyboardUpFunc(keyboard_up_event);
    glutMouseFunc(mouse_event);
    glutTimerFunc(updateMS, update_event, updateMS);
    glutIdleFunc(idle_event);
    glutMotionFunc(mouse_drag_event);
    glutPassiveMotionFunc(mouse_move_event);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
    // define global state
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glutIgnoreKeyRepeat(true);

    translateCamera(0, 0, 100);
    rotateCamera(0,-1.57,0);

    // run glut
    glutMainLoop();

//    while(true)
//    {
//        idle();
//        sleep(0.01);
//    }
}

int main(int argc, char **argv)
{

   // printf("\n");
   // printf("\t1 ....... Toggle Environment Drawing\n");
   // printf("\t2 ....... Toggle Sensing\n\n");
    SamplingSim::Instance(argc, argv);
    SamplingSim::Instance()->mainLoop();

    return 0;
}
