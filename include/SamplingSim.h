#include <stdio.h>
#include "World.h"
#include "Drone.h"

class SamplingSim
{
public:

    ~SamplingSim();

    static SamplingSim * Instance(int *argc=NULL, char **argv=NULL)
    {
        if(instance == NULL)
        {
            instance = new SamplingSim(argc, argv);
        }

        return instance;
    }

    void idle();
    void mainLoop();
    void hanldeKeyPressed(std::map<unsigned char, bool> &key, bool &updateKey);


private:
    World * world;
    Drone drone;
    static SamplingSim* instance;
    SamplingSim(int *argc, char **argv);


};
