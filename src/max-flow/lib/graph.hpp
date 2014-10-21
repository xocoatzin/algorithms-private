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
#include <stack>
#include <limits>
#include <iostream>

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

template <class TWeight = unsigned int>
class TGraph
{
public:
    typedef std::pair<int, int> Pair;
    typedef TWeight Weight;

    class Vertex
    {
    public:
        Vertex *next; // initialized and used in maxFlow() only
        int parent;
        int first;
        int ts;
        int distance;
        Weight weight;
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
        std::string to_name;
        std::string from_name;
        Weight weight;
    };

    class Path :
        public std::vector < Edge* >
    {
    public:
        typename Weight findMaxCapacity()
        {
            Weight c = std::numeric_limits<Weight>::max();
            for (auto &k : *this)
            {
                if (k->weight < c)
                    c = k->weight;
            }

            return c;
        }

        void print()
        {
            for (int i = 0; i < size() && at(i); i++)
            {
                if (!i) std::cout << "(" << at(i)->from_name << ")";
                std::cout << " -> (" << at(i)->to_name << ")";
            }
        }
    };

private:

    template <typename INDEX, typename TYPE>
    class HashVector :
        public std::unordered_map < INDEX, std::vector<TYPE> >
    {
    public:
        void append(INDEX key, TYPE val)
        {
            auto &i = find(key);
            if (i == end())
            {
                std::vector<TYPE> vec;
                vec.push_back(val);
                insert(value_type(key, vec));
            }
            else
            {
                auto &vec = i->second;
                vec.push_back(val);
            }
        }
    };

    typedef std::unordered_map < std::string, int > VIndex;
    typedef std::unordered_map < Pair, int > EIndex;

    typedef HashVector< int, int > ConnectionIndex;

    ConnectionIndex
        c_index;
    VIndex
        index;
    EIndex
        edg_index;
    std::vector < Vertex >
        vertices;
    std::vector < Edge >
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

            addEdge(from, tokens[0], to, tokens[1], weight);

            c_index.append(
                    (from),
                    (to)
                );
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

    void addEdge(int i, std::string i_name, int j, std::string j_name, TWeight w)
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
            fromI.from_name = i_name;
            fromI.to_name = j_name;
            fromI.weight = w;
            vertices[i].first = (int)edges.size();
            edges.push_back(fromI);
            id = (int)edges.size() - 1;

            edg_index.insert(EIndex::value_type(Pair(i, j), id));
        }
    };

private:
    bool findPath_recursive(
        int from,
        int to,
        std::stack < int > &s,
        std::unordered_map < int, bool > &seen)
    {
        auto i = c_index.find(from);
        if (i == c_index.end())
        {
            return false;
        }

        auto k = seen.find(from);
        if (k != seen.end())
        {
            return false;
        }
        seen.insert(std::unordered_map < int, bool >::value_type(from, true));

        auto &nodes = i->second;

        for (auto &n : nodes)
        {
            if (n == to)
            {
                s.push(to);
                return true;
            }
            else
            {
                if (findPath_recursive(n, to, s, seen))
                {
                    s.push(n);
                    return true;
                }
            }
        }

        k = seen.find(from);
        seen.erase(k);

        return false;
    }

public:
    Path findPath(int from, int to)
    {
        std::unordered_map < int, bool >
            seen;
        std::stack < int >
            stack;

        findPath_recursive(from, to, stack, seen);

        Path p;

        int _from = from;
        while (stack.size() > 0)
        {
            int _to = stack.top();
            stack.pop();

            Graph::Edge *edg = getEdge(_from, _to);
            p.push_back(edg);
            _from = _to;
        }

        return p;
    }

    Path findPath(std::string f, std::string t)
    {
        return findPath(findVertex(f), findVertex(t));
    }

    void print()
    {
        for (int i = 0; i < edges.size(); i++)
        {
            auto e = edges.at(i);
            std::cout
                << "(" << e.from_name << ")"
                << " -> (" << e.to_name << ") : "
                << e.weight
                << "\n";
        }
    }
};

typedef TGraph<unsigned int> Graph;



#endif
