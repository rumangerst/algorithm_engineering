#include <matmat.hpp>
#include <iostream>
#include <chrono>

#define LOOP_COUNT 1000000

using namespace std;

void print_result(double const * M, const int m, const int n)
{
  for(int i = 0; i < m; ++i)
  {
    for(int j = 0; j < n; ++j)
    {
      cout << M[j + n * i] << ", ";
    }
    cout << endl;
  }  
  
}

int main(int argc, char** argv)
{
  /*double A[4*3] =
  
  {
    1,2,3,
    10,1,4,
    8,2,-4,
    -1,-2,-3
  };
  int m = 4;
  int n = 3;
  int lda = 3;*/
  
  double A_T[4*3] =
  
  {
    1,10,8,-1,
    2,1,2,-2,
    3,4,-4,-3
  };
  
  
  int m_T = 3;
  int n_T = 4;  
  int lda_T = 4;
  
  double B[3*2] =
  
  {
    10,1,
    8,2,
    -1,2
  };
  int k = 2;
  int ldb = 2;
  
  double C[4*2] = 
  
  {
    0,0,
    0,0,
    0,0,
    0,0
  };
  int ldc = 2;
  
  auto start = chrono::high_resolution_clock::now();
  
  for(int i = 0; i < LOOP_COUNT; ++i)
  {
    matmat(1.0, true, A_T, m_T, n_T, lda_T, B, k, ldb, 1.0, C, ldc);
  }
  
  auto end = chrono::high_resolution_clock::now();
  auto elapsed_seconds = chrono::duration<double>(end-start).count();
  
  cout << "Runtime: " << elapsed_seconds << "s" << endl;
  
  print_result(C, 4, 2);
  
  return 0;
}
