#include "TSP.h"
#include <stdio.h>

vector<Entity* > TSP::GetShortestPath(vector<Entity* > to_visit)
{

    std::vector< Entity* > ent_in_order;
    vector<Vertex> shortest_path_vect;
    VertexListGraph g( to_visit.size() + 1 ); //plus one dummy node
    WeightMap weight_map(get(edge_weight, g));

    //populate map from vertices to Triangle3D structs
    std::map<Vertex,Entity*> verts_to_ent =  map_vertices_to_entities(to_visit, g);

    //printf("%d \n", verts_to_ent.size());
    create_connected_graph( g, weight_map, verts_to_ent);

    //solve metric tsp
    metric_tsp_approx_tour(g, back_inserter(shortest_path_vect));

    //make sure the shortext path starts and ends on the dummy vertex
    //assert( verts_to_ent.find(shortest_path_vect[0])==verts_to_ent.end() );
    //assert( verts_to_ent.find(shortest_path_vect[shortest_path_vect.size()-1])==verts_to_ent.end() );//assumes size>0

    //begin()+1 excludes the first vertex and end()-1 excludes the last, since they're both the dummy vertex
    for (vector<Vertex>::iterator itr = shortest_path_vect.begin()+1; itr != shortest_path_vect.end()-1; ++itr)
    {
        ent_in_order.push_back( verts_to_ent[*itr] );
    }

    return ent_in_order;

}

map<TSP::Vertex, Entity*> TSP::map_vertices_to_entities(vector<Entity*> entities, VertexListGraph &g)
{
    printf("O1");

    std::map<Vertex, Entity*> v_pmap;

    VItr vi, ve;
    int idx(-1);
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

    printf("OO ");
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
                double weight = 0.0;


                //the weights to and from the dummy vertex (indexes NULL) will be left as zero
                if (vmap.find(*src) != vmap.end() && vmap.find(*dest) != vmap.end()){
                    Vector<3> src_center = ( vmap[*src]->pos );
                    Vector<3> dest_center =( vmap[*dest]->pos );

                    weight = (src_center-dest_center)*(src_center-dest_center);
                    //printf("%f \n",weight);
                }
                else
                {
                    if(vmap.find(*src)!=vmap.end())
                    {
                        if(!vmap[*src]->start)
                        {
                            weight = 999999999;
                        }
                    }

                    if(vmap.find(*dest)!=vmap.end())
                    {
                        if(!vmap[*dest]->start)
                        {
                            weight = 999999999;
                        }
                    }
                }

                boost::tie(e, inserted) = add_edge(*src, *dest, g);
                //printf("%f \t",weight);


                wmap[e] = weight;
            }


        }
    }
   // printf("\n");
    //ROS_INFO("Found %lu vertices and %lu edges\n", num_vertices(g), num_edges(g));
}
