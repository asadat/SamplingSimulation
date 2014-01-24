#include "TSP.h"
#include <stdio.h>
//#define DIST(a,b)   sqrt((a-b)*(a-b))

double d(Vector<3> a, Vector<3> b)
{
    return sqrt((a-b)*(a-b));
}

void TSP::Sort(vector<Entity *> &list, Vector<3> s)
{

    vector<Entity *> sortedlist;


    while(!list.empty())
    {
        Entity* node = list.back();
        list.pop_back();

        if(sortedlist.empty())
        {
            sortedlist.push_back(node);
            continue;
        }

        bool flag = false;
        for(int i=0;i<sortedlist.size();i++)
        {
            if(d(node->pos,s)>d(sortedlist[i]->pos,s))
            {
                sortedlist.insert(sortedlist.begin()+i,node);
                flag = true;
                break;
            }

        }

        if(!flag)
            sortedlist.push_back(node);
    }

    //printf("size: %d\n", sortedlist.size());
    std::copy(sortedlist.begin(), sortedlist.end(), std::back_inserter(list));
    //printf("size: %d\n", list.size());

}

vector<Entity* > TSP::GetShortestPath_heu(vector<Entity* > to_visit)
{
    vector<Entity*> result;
    for(int i=0;i<to_visit.size();i++)
    {
        if(to_visit[i]->start)
        {
            result.push_back(to_visit[i]);
            to_visit.erase(to_visit.begin()+i);
            break;
        }
    }

    for(int i=0;i<to_visit.size();i++)
    {
        if(to_visit[i]->end)
        {
            result.push_back(to_visit[i]);
            to_visit.erase(to_visit.begin()+i);
            break;
        }
    }

    Sort(to_visit,result[1]->pos);
    while(!to_visit.empty())
    {
        Entity * en = to_visit.back();
        to_visit.pop_back();
        int idx=-1;
        double mindist = 999999999;
        for(int i=0; i+1<result.size();i++)
        {
            //Vector<3> v = ((en->pos-result[i]->pos)^(en->pos-result[i+1]->pos));
            //double dist = sqrt(v*v)/sqrt((result[i]->pos-result[i+1]->pos)*(result[i]->pos-result[i+1]->pos));
            double dist = sqrt((en->pos-result[i]->pos)*(en->pos-result[i]->pos))+sqrt((en->pos-result[i+1]->pos)*(en->pos-result[i+1]->pos))-sqrt((result[i]->pos-result[i+1]->pos)*(result[i]->pos-result[i+1]->pos));
            if(dist<mindist)
            {
                mindist = dist;
                idx = i;
            }

        }

        result.insert(result.begin()+idx+1,en);
    }
    return result;
}

vector<Entity* > TSP::GetShortestPath(vector<Entity* > to_visit)
{
    std::vector< Entity* > ent_in_order;
    vector<Vertex> shortest_path_vect;
    VertexListGraph g( to_visit.size());
    WeightMap weight_map(get(edge_weight, g));

    //populate map from vertices to Triangle3D structs
    std::map<Vertex,Entity*> verts_to_ent =  map_vertices_to_entities(to_visit, g);

    //printf("%d \n", verts_to_ent.size());
    create_connected_graph( g, weight_map, verts_to_ent);

    //solve metric tsp
    metric_tsp_approx_tour(g, back_inserter(shortest_path_vect));

    //make sure the shortext path starts and ends on the dummy vertex
   // assert( verts_to_ent.find(shortest_path_vect[0])==verts_to_ent.end() );
   // assert( verts_to_ent.find(shortest_path_vect[shortest_path_vect.size()-1])==verts_to_ent.end() );//assumes size>0

    //begin()+1 excludes the first vertex and end()-1 excludes the last, since they're both the dummy vertex
    for (vector<Vertex>::iterator itr = shortest_path_vect.begin(); itr != shortest_path_vect.end()-1; ++itr)
    {
        ent_in_order.push_back( verts_to_ent[*itr] );
    }

    TwoOptOptimization(ent_in_order);
    //TwoOptOptimization(ent_in_order);
    //TwoOptOptimization(ent_in_order);
    //TwoOptOptimization(ent_in_order);
    return ent_in_order;




}

void TSP::TwoOptOptimization(vector<Entity *> &path)
{
    for(int i=0; i+3<path.size(); i++)
    {
        if(d(path[i]->pos,path[i+2]->pos) + d(path[i+2]->pos,path[i+1]->pos) + d(path[i+1]->pos,path[i+3]->pos) <
                d(path[i]->pos,path[i+1]->pos) + d(path[i+1]->pos,path[i+2]->pos) + d(path[i+2]->pos,path[i+3]->pos))
        {
            Entity* i_2 = path[i+2];
            path.erase(path.begin()+i+2);
            path.insert(path.begin()+i+1, i_2);
        }
    }
}

map<TSP::Vertex, Entity*> TSP::map_vertices_to_entities(vector<Entity*> entities, VertexListGraph &g)
{
   // printf("O1");

    std::map<Vertex, Entity*> v_pmap;

    VItr vi, ve;
    int idx(0);
    for (boost::tie(vi, ve) = vertices(g); vi != ve; ++vi)
    {
        Vertex v(*vi);
        if (idx != -1) //we leave the first vertex unmapped, since it's a dummy vertex
        {
            v_pmap[v] = entities[idx];
        }
//        else
//        {
//            v_pmap[v] = NULL;
//            //v_pmap[v]->pos = makeVector(999999,0,0)
//        }

        idx++;
    }

    return v_pmap;
}

void TSP::create_connected_graph(VertexListGraph &g, WeightMap wmap, std::map<Vertex,Entity*> vmap)
{

    Edge e;
    bool inserted;

    //printf("OO ");
    pair<VItr, VItr> verts(vertices(g));
    for (VItr src(verts.first); src != verts.second; src++)
    {
        //printf("O1 ");
        for (VItr dest(src); dest != verts.second; dest++)
        {
            //printf("O2 ");
            if (dest != src)
            {
                //printf("O3 ");
                double weight = 0;


                //the weights to and from the dummy vertex (indexes NULL) will be left as zero
                if (vmap.find(*src) != vmap.end() && vmap.find(*dest) != vmap.end()){
                    Vector<3> src_center = ( vmap[*src]->pos );
                    Vector<3> dest_center =( vmap[*dest]->pos );

                    weight = sqrt((src_center-dest_center)*(src_center-dest_center));
                    //printf("%f \n",weight);
                }
                else
                {
                    if(vmap.find(*src)!=vmap.end())
                    {
                        if(!vmap[*src]->start && !vmap[*src]->end )
                        {
                            weight = 999999999999999;
                        }
                    }

                    if(vmap.find(*dest)!=vmap.end())
                    {
                        if(!vmap[*dest]->start && !vmap[*dest]->end)
                        {
                            weight = 9999999999999999;
                        }
                    }
                }

                boost::tie(e, inserted) = add_edge(*src, *dest, g);
                wmap[e] = weight;
                //printf("%f \t",weight);
                //boost::tie(e, inserted) = add_edge(*dest, *src, g);
                //wmap[e] = weight;

               // printf("%f \t",weight);


            }


        }
    }
   // printf("\n");
    //ROS_INFO("Found %lu vertices and %lu edges\n", num_vertices(g), num_edges(g));
}
