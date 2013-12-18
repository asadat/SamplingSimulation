#include <vector>

class Visualizer
{
public:
    Visualizer();
    ~Visualizer();

    virtual void glDraw() = 0;
    static void glDrawAll();

private:

    static std::vector<Visualizer*> objects;
};


