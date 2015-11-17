#include <matmat.hpp>
#include <iostream>
#include <chrono>

#define LOOP_COUNT 1

using namespace std;

void print_result(double const * M, const int m, const int n, const int ld)
{
  for(int i = 0; i < m; ++i)
  {
    for(int j = 0; j < n; ++j)
    {
      cout << M[j + ld * i] << ", ";
    }
    cout << endl;
  }  
  
}

int main(int argc, char** argv)
{

  /*double A[3 * (2 + 6)] __attribute__((aligned(64))) = 
  {
    3,1,0,0,0,0,0,0,
    2,0,0,0,0,0,0,0,
    1,2,0,0,0,0,0,0
    
  };
  double B[3 * (2 + 6)] __attribute__((aligned(64))) =
  {
    1,2,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,
    4,0,0,0,0,0,0,0
  };
  double C[2 * (2+6)] __attribute__((aligned(64))) =
  {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
  };
  
  int m = 3;
  int n = 2;
  int k = 2;
  
  int lda = 8;
  int ldb = 8;
  int ldc = 8;
  
  double alpha = 1.0;
  double beta = 1.0;
  
  bool trans = true;*/

   
  /*double A[4*(3 + 5)] __attribute__((aligned(64))) =
  
  {
    1,2,3,0,0,0,0,0,
    10,1,4,0,0,0,0,0,
    8,2,-4,0,0,0,0,0,
    -1,-2,-3,0,0,0,0,0
  };
  
  int m = 4;
  int n = 3;
  int lda = 8;
  bool trans = false;
  
  cout << "Testing C = alpha*C + beta*A*B" << endl;*/
  

  double A[3*(4+4)] __attribute__((aligned(64))) =
  
  {
    1,10,8,-1,0,0,0,0,
    2,1,2,-2,0,0,0,0,
    3,4,-4,-3,0,0,0,0
  };
  
  
  int m = 3;
  int n = 4;  
  int lda = 8;
  bool trans = true;
  
  cout << "Testing C = alpha*C + beta*A^T*B" << endl;
  
  
  double B[3*(2 + 6)] __attribute__((aligned(64))) =
  
  {
    10,1,0,0,0,0,0,0,
    8,2,0,0,0,0,0,0,
    -1,2,0,0,0,0,0,0
  };
  int k = 2;
  int ldb = 8;
  
  double C[4*(2 + 6)] __attribute__((aligned(64))) = 
  
  {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
  };
  int ldc = 8;

  //Actual test matrices
  /*int m = 200;
  int n = 500;
  int k = 100;
  int lda = 512;
  int ldb = 128;
  int ldc = 128;
  bool trans = false;
  
  double A [m * lda] __attribute__((aligned(64)));
  double B [n * ldb] __attribute__((aligned(64)));
  double C [m * ldc] __attribute__((aligned(64)));
  
  for(int i = 0; i < m * lda; ++i)
    A[i] = i;
    
  for(int i = 0; i < n * ldb; ++i)
    B[i] = i - 600;
    
  for(int i = 0; i < m * ldc; ++i)
    C[i] = 0;*/
    
  
  auto start = chrono::high_resolution_clock::now();
  
  for(int i = 0; i < LOOP_COUNT; ++i)
  {
    matmat(1.0, trans, A, m, n, lda, B, k, ldb, 1.0, C, ldc);
  }
  
  auto end = chrono::high_resolution_clock::now();
  auto elapsed_seconds = chrono::duration<double>(end-start).count();
  
  
  
  print_result(C, !trans ? m : n, k, ldc);
  cout << "Runtime: " << elapsed_seconds << "s" << endl;
  
  return 0;
}
