#include "matmat.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

/**
  computes C = alpha * op(A) * B + beta * C
  op(A) = [A | A^T]
*/
void matmat(double alpha, bool trans, double const* a,
      	    int m, int n, int lda, double const * b,
      	    int k, int ldb, double beta, double * __restrict__ c, int ldc) 
{
  if(m <= 0 || n <= 0 || k <= 0)
    std::exit(EXIT_FAILURE);

  if (trans) 
  { 
    //Each column in A^T
    for(int x = 0; x < m; ++x)
    {
      double const * a_row = a + x * lda;
      double const * b_row = b + ldb * x;
      
      // Each row
      for (int i = 0; i < n; ++i)
      {
      
        double * c_row = c + ldc * i;        
        
        #pragma omp simd aligned(c:64)
	      #pragma vector aligned
        for(int j = 0; j< k; ++j)
        {
          c_row[j] *= beta;
        }
        
        #pragma omp simd aligned(a, b, c:64)
	      #pragma vector aligned
        for(int j = 0; j < k; ++j)
        {
          //cout << a_row[i] << " * " << b_row[j]<<endl;
          c_row[j] += a_row[i] * b_row[j];
        
        }            
        
        //cout << "--" << endl;
        
        
      }
      
      //cout << "--" << endl;
    }
  } 
  else 
  {
    
    for(int i = 0; i < m; ++i) 
    {  // row
    
      double * c_row = c + ldc * i;
      double const * a_row = a + lda * i;
      
      // Calculate c * beta
      #pragma omp simd aligned(c:64)
	    #pragma vector aligned
      for(int x = 0; x < k; ++x)
      {
        c_row[x] *= beta;
      }
      
      // Multiply
	    for(int x = 0; x < n; ++x) 
	    {  
	      // multiply operations
	      #pragma omp simd aligned(a, b, c:64)
	      #pragma vector aligned
	      for(int j = 0; j < k; ++j)  //column
	      {
	        double const * b_row =  b + ldb * x;
	        
		      c_row[j] += alpha * a_row[x] * b_row[j];
        }
	    }
    }
  }
}


