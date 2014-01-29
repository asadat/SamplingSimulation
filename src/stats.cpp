#include <stdio.h>
#include <map>
#include <vector>
#include <math.h>

using namespace std;

double mean(vector<double> &v)
{
    if(v.empty())
        return 0;

    double sum=0;
    for(int i=0; i<v.size(); i++)
    {
        sum += v[i];
    }

    return sum/v.size();
}

double var(vector<double> &v)
{
    if(v.size() <= 1)
        return 0;

    double m=mean(v);
    double difsum;
    for(int i=0; i<v.size(); i++)
    {
        difsum = (m-v[i])*(m-v[i]);
    }

    return sqrt(difsum/(v.size()-1));
}

int main(int argc, char **argv)
{
    map<int, vector<double> > data;

    int cells_n = -1;
    float val = 0;

    FILE * f=fopen(argv[1], "r+");


    while(fscanf(f,"%d %f\n",&cells_n, &val) != EOF)
    {
        if(cells_n < 0)
            break;

        //printf("%d %f\n", cells_n, val);
        if(data.find(cells_n) == data.end())
        {
            vector<double> v;
            v.push_back(val);
            data[cells_n] = v;
        }
        else
        {
            data[cells_n].push_back(val);
        }
    }

    fclose(f);

    map<int, vector<double> >::iterator it;
    for(it = data.begin(); it!=data.end(); it++)
    {
        printf("%d %f %f\n", it->first, mean(it->second), var(it->second));
    }
    return 0;
}
