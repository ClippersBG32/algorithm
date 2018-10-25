#include<cstdio>
#include<iostream>
#define MAXN 100
using namespace std;

const double INFINITY = 0x3f3f3f3f;

//the simplex matrix
double A[][7] = {
       {0, -7, 7, -2, -1, -6, 0},
       {-3, 3, -1, 1, -2, 0, 0},
       {4, 2, 1, 0 , 1, 1, 0},
       {12, -1, 3, 0, -3, 0, 1}
};

int m=3; //the number of constraints
int n=6; //the number of variants
int base[MAXN] = {0}; //the basic vector
double delta[MAXN] = {0};

void pivot(int l, int e){
    //scale the l-th line
    double scale = A[l][e];
    for(int j = 0; j <= n; ++j){
        A[l][j] = A[l][j] / scale;
    }


    //all other lines minus the l-th line
    for(int i = 0; i <= m; ++i){
       if(i == l){
          continue;
        }
       scale = A[i][e];
       for(int j = 0; j <= n; ++j){
           A[i][j] = A[i][j] - scale*A[l][j];
       }
    }

    //Calculating x
    base[l] = 0;
    base[e] = 1;
}

static bool dual_simplex(void){
    //get a dual feasible basis.
    //print the matrix
    cout << "The matrix here is" << endl;
    for(int i= 0; i<= m; ++i){
       for(int j=0; j<= n; ++j){
          cout << A[i][j]<< ' ';
       }
       cout << endl;
    }

    //initialize the matrix
    double scale = 0;
    for(int i = 1, j = 1; i <=m; ++i){
        for(j; base[j] == 0; ++j );
        scale = A[i][j];
        for(int l=0; l<= n; ++l){
          A[i][l] = A[i][l] / scale;
        }

        for(int p=0; p<=m; ++p){
          if(p == i){
            continue;
          }
          scale = A[p][j];
          for(int l=0; l<=n; ++l){
              A[p][l] = A[p][l] - scale * A[i][l];
          }
        }
        ++j;
    }

    while(true){
        //find minimum b
        cout << "The matrix here is" << endl;
        for(int i= 0; i<= m; ++i){
           for(int j=0; j<= n; ++j){
              cout << A[i][j]<< ' ';
           }
           cout << endl;
        }
        int min = INFINITY;
        int min_pos = 0;
        int l, e;
        for(int i = 1; i <= m; ++i){
           if(A[i][0] < min){
              min = A[i][0];
              min_pos = i;
           }
        }

        if(min >= 0){
           return true;
        }
        //choose a b < 0
        l = min_pos;

        for(int j = 1; j <= n; ++j){
            if(A[l][j] < 0){
              delta[j] = - A[0][j] / A[l][j];
            }
            else{
              delta[j] = INFINITY;
            }
        }

        //choose an index e that minimizes delta j
        min = INFINITY;
        e = 1;
        for(int i = 2; i <= n; ++i){
           if(delta[i] < min){
              min = delta[i];
              e = i;
           }
        }

        if(delta[e] == INFINITY){
           return false;
        }
        pivot(l, e);
    }
}

int main(){
  base[3] = 1;
  base[5] = 1;
  base[6] = 1;

  if(dual_simplex()){
    cout << "The optimum solution is " << -A[0][0] << endl;
  }
  else{
    cout << "No feasible solution" << endl;
  }
  return 0;
}
