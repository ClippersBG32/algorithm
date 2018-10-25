#include<iostream>
#include<cstdio>
#include "ford_fulkerson.h"
#include "push_relabel.h"
using namespace std;

Ford_Fulkerson ff;
pair<int, int> cyber[N];
PushRelabel pr;
int row[N];
int col[N];
int row_sum[N];
int col_sum[N];

void problem1(){
     freopen("problem1.data", "r", stdin);
     int n, m;
     int lower, upper;
     Ford_Fulkerson ff;
     while(cin >> n >> m){
        for(int i= 0; i< n; ++i){
           cin >> cyber[i].first >> cyber[i].second;
        }

        lower = 0;
        upper = n;
        int mid;
        ff.Initialize(0, n+m+1, n+m+2);
        while(lower < upper){
           mid = (lower + upper) / 2;

           //add link between super source s and job
           for(int i= 1; i <= n; ++i){
              ff.AddEdge(0, i, 1);
           }

           //add link between computer super target t
           for(int i= n+1; i <= n+m; ++i){
             ff.AddEdge(i, n+m+1, mid);
           }

           //add link between job and Computer
           for(int i = 1; i <= n; ++i){
              ff.AddEdge(i, n+cyber[i-1].first, 1);
              ff.AddEdge(i, n+cyber[i-1].second, 1);
           }

           ff.Maxflow();
           int f = ff.get_maxflow();

           if(f == n){
             upper = mid;
           }
           else{
             lower = mid + 1;
           }
           ff.clearflow();
        }
      cout << "The minimal max job load is " << upper << endl;
     }

}


void problem2(){
     freopen("problem2.data", "r", stdin);
     int m, n;
     int cap;
     int tmp;
     while(cin >> n >> m){
         pr.clear();

         tmp = 0;
         pr.Initialize(0, n+m+1, n+m+2);
         for(int i = 1; i <= n; ++i){
            cin >> cap;
            tmp += cap;
            row_sum[i-1] = cap;
            pr.AddEdge(0, i, cap);
         }

         for(int i = 1; i <= m; ++i){
            cin >> cap;
            col_sum[i-1] = cap;
            pr.AddEdge(n+i, n+m+1, cap);
         }

         for(int i = 1; i <= n; ++i){
            for(int j = 1; j <= m; ++j){
               pr.AddEdge(i, n+j, 1);
            }
         }

        pr.Maxflow();
        int f = pr.get_maxflow();


        bool check = true;
        if(f == tmp){
           memset(col, 0, sizeof(col));
           for(int i= 1; i <=n ; ++i){
              memset(row, 0, sizeof(row));
              pr.GetFlowOfNode(i, row, n+1);
              int sum = 0;
              for(int j = 0; j < m; ++j){
                 sum += row[j];
                 col[j] += row[j];
          }
              if(sum != row_sum[i-1]){
                check = false;
              }
        }

        for(int i=0; i<m ; ++i){
           if(col[i] != col_sum[i]){
             check = false;
           }
        }
        for(int i=0; i<m ; ++i){
        }

        if(check){
           cout << "Outputs checked." << endl;
        }
        else{
           cout << "Wrong answer." << endl;
        }
      }
       else{
         cout << "No matrix meets the condition." << endl;
       }
    }

}

int main(){
    problem1();
    //problem2();
    return 0;
}
