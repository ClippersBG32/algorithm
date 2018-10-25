#ifndef EDGE_H
#define EDGE_H
struct edge{
    int inverse_index, v; //G[u][inverse index] is the inverse edge (v,u)
                          // v is the index of target node of edge (u, v)
    int cap, flow;
};

const int N = 6500;
const int INF = 0x3f3f3f3f;
#endif
