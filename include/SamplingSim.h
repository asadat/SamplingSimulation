#ifndef _SAMPLINGSIM
#define _SAMPLINGSIM

#include <stdio.h>
#include "World.h"
#include "Drone.h"

class SamplingSim
{
public:

    ~SamplingSim();

    static SamplingSim * Instance(int argc=0, char **argv=NULL)
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

    SamplingSim(int argc, char **argv);
    static SamplingSim* instance;

    World * world;
    Drone drone;

};

#endif
