/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistanceribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistanceribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistanceribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distanceribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef _CV_GRAPH_H_
#define _CV_GRAPH_H_

#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../../common/io.h"
#include "../../common/string.h"

using namespace tools;

template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<typename S, typename T> struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T> & v) const
        {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
}


typedef unsigned char uchar;

template <class TWeight = double>
class TGraph
{
public:
    typedef std::pair<int, int> Pair;

    class Vertex
    {
    public:
        Vertex *next; // initialized and used in maxFlow() only
        int parent;
        int first;
        int ts;
        int distance;
        TWeight weight;
        uchar t;
        std::string name;

        Vertex() :
            next(NULL),
            parent(0),
            first(0),
            ts(0),
            weight(0),
            t(0)
        {};

        Vertex(std::string name) :
            next(NULL),
            parent(0),
            first(0),
            ts(0),
            weight(0),
            t(0),
            name(name)
        {};
    };

    class Edge
    {
    public:
        int to;
        int next;
        TWeight weight;
    };

private:
    typedef std::unordered_map < std::string, int > VIndex;
    typedef std::unordered_map < Pair, int > EIndex;

    VIndex
        index;
    EIndex
        edg_index;
    std::vector<Vertex>
        vertices;
    std::vector<Edge>
        edges;
    TWeight
        flow;

public:
    //TGraph() :
    //    flow(0)
    //{};
    //
    //TGraph(unsigned int vtxCount, unsigned int edgeCount) :
    //    TGraph()
    //{
    //    vertices.reserve(vtxCount);
    //    edges.reserve(edgeCount);
    //};

    TGraph(std::string filename)
    {
        std::cout << "Reading [" << filename << "]" << std::endl;

        Reader reader(filename);

        std::string line;
        std::size_t line_id;
        while (reader.getLine(line, line_id))
        {
            std::vector<std::string> tokens = tools::split(line, ",");
            if (tokens.size() != 2)
            {
                std::cout << "ERROR reading line (bad format): " << line_id << std::endl;
                continue;
            }

            int weight = 0;
            try
            {
                weight = std::stoi(tokens[1]);
            }
            catch (...)
            {
                std::cout << "ERROR reading in line (bad format of weight): " << line_id << std::endl;
                continue;
            }

            tokens = tools::split(tokens[0], "->");

            if (tokens.size() != 2)
            {
                std::cout << "ERROR reading line (you need to include 2 vetices): " << line_id << std::endl;
                continue;
            }

            int
                from = addVertex(tokens[0]),
                to = addVertex(tokens[1]);

            addEdge(from, to, weight);
        }
    };

    ~TGraph()
    {};

    int addVertex(std::string name)
    {
        int id = findVertex(name);
        if (id < 0)
        {
            Vertex v(name);
            vertices.push_back(v);
            id = (int)vertices.size() - 1;

            index.insert(VIndex::value_type(name, id));
        }

        return id;
    };

    int findVertex(std::string name)
    {
        auto &i = index.find(name);
        if (i != index.end())
            return i->second;
        else
            return -1;
    }

    Vertex* getVertex(int id)
    {
        if (id < 0 || id >= (int)vertices.size())
            return NULL;

        return &(vertices[id]);
    }

    Vertex* getVertex(std::string name)
    {
        return getVertex(findVertex(name));
    }

    int countVertex()
    {
        return vertices.size();
    }

    int findEdge(Pair id)
    {
        auto &i = edg_index.find(id);
        if (i != edg_index.end())
            return i->second;
        else
            return -1;
    }

    int findEdge(int f, int t)
    {
        return findEdge(Pair(f, t));
    }

    int findEdge(std::string f, std::string t)
    {
        int
            fid = findVertex(f),
            tid = findVertex(t);

        if (fid < 0 || tid < 0) return -1;

        return findEdge(fid, tid);
    }

    Edge* getEdge(int id)
    {
        if (id < 0 || id >= (int)edges.size())
            return NULL;

        return &(edges[id]);
    }

    Edge* getEdge(Pair id)
    {
        return getEdge(findEdge(id));
    }

    Edge* getEdge(int f, int t)
    {
        return getEdge(findEdge(f, t));
    }

    Edge* getEdge(std::string f, std::string t)
    {
        return getEdge(findEdge(f, t));
    }

    int countEdges()
    {
        return edges.size();
    }

    void addEdge(int i, int j, TWeight w)
    {
        if (!(i >= 0 && i < (int)vertices.size())) return;
        if (!(j >= 0 && j < (int)vertices.size())) return;
        if (!(w >= 0)) return;
        if (!(i != j)) return;

        int id = findEdge(Pair(i, j));
        if (id < 0)
        {
            Edge
                fromI;

            fromI.to = j;
            fromI.next = vertices[i].first;
            fromI.weight = w;
            vertices[i].first = (int)edges.size();
            edges.push_back(fromI);
            id = (int)edges.size() - 1;

            edg_index.insert(EIndex::value_type(Pair(i, j), id));
        }
    };

    TWeight maxFlow();
};

typedef TGraph<double> Graph;

template <class TWeight>
TWeight TGraph<TWeight>::maxFlow()
{
    const int TERMINAL = -1, ORPHAN = -2;
    Vertex stub, *nilNode = &stub, *first = nilNode, *last = nilNode;
    int curr_ts = 0;
    stub.next = nilNode;
    Vertex *vtxPtr = &vertices[0];
    Edge *edgePtr = &edges[0];

    std::vector<Vertex*> orphans;

    // initialize the active queue and the graph vertices
    for( int i = 0; i < (int)vertices.size(); i++ )
    {
        Vertex* v = vtxPtr + i;
        v->ts = 0;
        if( v->weight != 0 )
        {
            last = last->next = v;
            v->distance = 1;
            v->parent = TERMINAL;
            v->t = v->weight < 0;
        }
        else
            v->parent = 0;
    }
    first = first->next;
    last->next = nilNode;
    nilNode->next = 0;

    // run the search-path -> augment-graph -> restore-trees loop
    for(;;)
    {
        Vertex* v, *u;
        int e0 = -1, ei = 0, ej = 0;
        TWeight minWeight, weight;
        uchar vt;

        // grow S & T search trees, find an edge connecting them
        while( first != nilNode )
        {
            v = first;
            if( v->parent )
            {
                vt = v->t;
                for( ei = v->first; ei != 0; ei = edgePtr[ei].next )
                {
                    if( edgePtr[ei^vt].weight == 0 )
                        continue;
                    u = vtxPtr+edgePtr[ei].to;
                    if( !u->parent )
                    {
                        u->t = vt;
                        u->parent = ei ^ 1;
                        u->ts = v->ts;
                        u->distance = v->distance + 1;
                        if( !u->next )
                        {
                            u->next = nilNode;
                            last = last->next = u;
                        }
                        continue;
                    }

                    if( u->t != vt )
                    {
                        e0 = ei ^ vt;
                        break;
                    }

                    if( u->distance > v->distance+1 && u->ts <= v->ts )
                    {
                        // reassign the parent
                        u->parent = ei ^ 1;
                        u->ts = v->ts;
                        u->distance = v->distance + 1;
                    }
                }
                if( e0 > 0 )
                    break;
            }
            // exclude the vertex from the active list
            first = first->next;
            v->next = 0;
        }

        if( e0 <= 0 )
            break;

        // find the minimum edge weight along the path
        minWeight = edgePtr[e0].weight;
        //assert( minWeight > 0 );
        // k = 1: source tree, k = 0: destination tree
        for( int k = 1; k >= 0; k-- )
        {
            for( v = vtxPtr+edgePtr[e0^k].to;; v = vtxPtr+edgePtr[ei].to )
            {
                if( (ei = v->parent) < 0 )
                    break;
                weight = edgePtr[ei^k].weight;
                minWeight = std::min(minWeight, weight);
                //assert( minWeight > 0 );
            }
            weight = fabs(v->weight);
            minWeight = std::min(minWeight, weight);
            //assert( minWeight > 0 );
        }

        // modify weights of the edges along the path and collect orphans
        edgePtr[e0].weight -= minWeight;
        edgePtr[e0^1].weight += minWeight;
        flow += minWeight;

        // k = 1: source tree, k = 0: destination tree
        for( int k = 1; k >= 0; k-- )
        {
            for( v = vtxPtr+edgePtr[e0^k].to;; v = vtxPtr+edgePtr[ei].to )
            {
                if( (ei = v->parent) < 0 )
                    break;
                edgePtr[ei^(k^1)].weight += minWeight;
                if( (edgePtr[ei^k].weight -= minWeight) == 0 )
                {
                    orphans.push_back(v);
                    v->parent = ORPHAN;
                }
            }

            v->weight = v->weight + minWeight*(1-k*2);
            if( v->weight == 0 )
            {
               orphans.push_back(v);
               v->parent = ORPHAN;
            }
        }

        // restore the search trees by finding new parents for the orphans
        curr_ts++;
        while( !orphans.empty() )
        {
            Vertex* v2 = orphans.back();
            orphans.pop_back();

            int d, mindistance = INT_MAX;
            e0 = 0;
            vt = v2->t;

            for( ei = v2->first; ei != 0; ei = edgePtr[ei].next )
            {
                if( edgePtr[ei^(vt^1)].weight == 0 )
                    continue;
                u = vtxPtr+edgePtr[ei].to;
                if( u->t != vt || u->parent == 0 )
                    continue;
                // compute the distanceance to the tree root
                for( d = 0;; )
                {
                    if( u->ts == curr_ts )
                    {
                        d += u->distance;
                        break;
                    }
                    ej = u->parent;
                    d++;
                    if( ej < 0 )
                    {
                        if( ej == ORPHAN )
                            d = INT_MAX-1;
                        else
                        {
                            u->ts = curr_ts;
                            u->distance = 1;
                        }
                        break;
                    }
                    u = vtxPtr+edgePtr[ej].to;
                }

                // update the distanceance
                if( ++d < INT_MAX )
                {
                    if( d < mindistance )
                    {
                        mindistance = d;
                        e0 = ei;
                    }
                    for( u = vtxPtr+edgePtr[ei].to; u->ts != curr_ts; u = vtxPtr+edgePtr[u->parent].to )
                    {
                        u->ts = curr_ts;
                        u->distance = --d;
                    }
                }
            }

            if( (v2->parent = e0) > 0 )
            {
                v2->ts = curr_ts;
                v2->distance = mindistance;
                continue;
            }

            /* no parent is found */
            v2->ts = 0;
            for( ei = v2->first; ei != 0; ei = edgePtr[ei].next )
            {
                u = vtxPtr+edgePtr[ei].to;
                ej = u->parent;
                if( u->t != vt || !ej )
                    continue;
                if( edgePtr[ei^(vt^1)].weight && !u->next )
                {
                    u->next = nilNode;
                    last = last->next = u;
                }
                if( ej > 0 && vtxPtr+edgePtr[ej].to == v2 )
                {
                    orphans.push_back(u);
                    u->parent = ORPHAN;
                }
            }
        }
    }
    return flow;
}


#endif
