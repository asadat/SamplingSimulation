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
    map<int, map<int,vector<double> > > data[4];

    int cells_n = -1;
    int perc = -1;
    int strategy = -1;
    float val = 0;
    char stra=-1;

    FILE * f=fopen(argv[1], "r+");


    while(fscanf(f,"%c %d %d %d %f\n", &stra, &strategy, &perc, &cells_n, &val) != EOF)
    {
        if(cells_n < 0)
            break;

        //printf("%c %d %d %d %f\n", stra, strategy, perc, cells_n, val);
        if(data[strategy].find(perc) == data[strategy].end())
        {
            map<int, vector<double> > num_val;
            vector<double> v;
            v.push_back(val);
            //num_val.insert(pair<int, vector<double> >(cells_n, num_val));
            num_val[cells_n] = v;
            data[strategy][perc] = num_val;
            //data[cells_n] = v;
        }
        else
        {
            //data[cells_n].push_back(val);
            if(data[strategy][perc].find(cells_n) == data[strategy][perc].end())
            {
                vector<double> vals;
                vals.push_back(val);
                data[strategy][perc][cells_n] = vals;
            }
            else
            {
                data[strategy][perc][cells_n].push_back(val);
            }
        }
    }

    fclose(f);

    map<int, vector<double> >::iterator it;
    map<int, map<int,vector<double> > >::iterator itt;

    for(int i=0; i<4; i++)
    {
        for(itt = data[i].begin(); itt!=data[i].end(); itt++)
        {
            for(it = itt->second.begin(); it!=itt->second.end(); it++)
            {
                printf("%d\t%d\t%d\t%f\t%f\n", i, itt->first, it->first, mean(it->second), var(it->second));
            }
        }
    }
    return 0;
}
