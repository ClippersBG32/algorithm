#include<iostream>
#include<vector>
#include<algorithm>
#include "edge.h"
using namespace std;

class PushRelabel{
  public:
    ~PushRelabel();

    void Initialize(int s, int t, int n);
    void Maxflow();
    int get_maxflow();

    void AddEdge(int u, int v, int cap);
    void GetFlowOfNode(int u, int* a, int offset);
    int GetNodeWithPositiveExcess();
    int GetRelabelEdge(int v);

    void clear();

  private:
    int maxflow;
    int h[N]; //label of each node
    int s, t; //source and target
    int n; //number of vertices
    vector<edge> G[N]; //edges
    int Ex[N];    //the excess of All node
};

PushRelabel::~PushRelabel(){
   clear();
}

void PushRelabel::clear(){
  this->maxflow = 0;
  for(int i= 0; i < n; ++i){
      G[i].clear();
  }
  memset(h, 0, sizeof(h));
  memset(Ex, 0, sizeof(Ex));

}

int PushRelabel::get_maxflow(){
   return this->maxflow;
}

void PushRelabel::Initialize(int s, int t, int n){
   this->s = s;
   this->t = t;
   this->n = n;
   memset(h, 0, sizeof(h));
}

void PushRelabel::AddEdge(int u, int v, int cap){
     int u_index = G[u].size();
     int v_index = G[v].size();
     G[u].push_back((edge){v_index, v, cap, 0});
     G[v].push_back((edge){u_index, u, 0, 0});
}

void PushRelabel::GetFlowOfNode(int u, int *a, int offset) {
    int len = G[u].size();
    for(int i= 0 ; i < len; ++i){
        if(G[u][i].cap == 0)
          continue;
        a[G[u][i].v-offset]= G[u][i].flow;
    }

}

int PushRelabel::GetNodeWithPositiveExcess(){
    for(int i =0; i < n-1; ++i){
       if(Ex[i] > 0){
          return i;
       }
    }
    return -1;
}

int PushRelabel::GetRelabelEdge(int v){
    int len = G[v].size();
    for(int i = 0; i < len; ++i){
        if(G[v][i].cap > G[v][i].flow && h[v] > h[G[v][i].v]){
           return i;
        }
    }
    //no edge satisfies the constraint.
    return -1;
}

void PushRelabel::Maxflow(){
   memset(h, 0, sizeof(h));
   h[s] = n;
   maxflow = 0;
   memset(Ex, 0, sizeof(Ex));

   int len = G[s].size();
   for(int i= 0; i < len; ++i){
       edge& e = G[s][i];
       e.flow = e.cap;
       G[e.v][e.inverse_index].flow -= e.cap;
       Ex[s] -= e.flow;
       Ex[e.v] = e.flow;
   }

   int v, w;
   int bottleneck;
   while((v = GetNodeWithPositiveExcess()) > 0){

    if((w =GetRelabelEdge(v))>= 0){
    //there exists an edge (v, w) h(v) > h(w)
      edge& e = G[v][w];

      bottleneck = min(Ex[v], e.cap - e.flow);
      e.flow += bottleneck;
      G[e.v][e.inverse_index].flow -= bottleneck;

      Ex[v] -= bottleneck;
      Ex[e.v] += bottleneck;
   }
   else{
     h[v] += 1;
   }
   if(h[v] >= 2*n){
      break;
   }
 }
 maxflow = Ex[t];
}
