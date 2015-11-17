#include <matmat.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;

void matmat(double alpha, bool trans, double const* a,
	    int m, int n, int lda, double const * b,
	    int k, int ldb, double beta, double * c, int ldc)
{  
    
    if(m <= 0 || n <= 0 || k <= 0)
        exit(EXIT_FAILURE);    
  
    
    if(trans)
    {
        int h = m;
        m = n;
        n = h;
    }
    
  for(int i = 0; i < m; ++i) //row
  {
    for(int j = 0; j < k; ++j) //column
    {     
      // Multiply the row of A with column of B to get the value for C      
           
        // Vectorizable
      double w = 0;
      
      for(int x = 0; x < n; ++x)
      {
          const double _a = trans ? a[i + x * lda] : a[x + i * lda];
          const double _b = b[j + x * ldb];
          
          
        w += alpha * _a * _b;
      }
      
      c[j + i* ldc] = beta * c[j + i* ldc] + w;
      
      //Not vectorizable
      /*c[j + i* ldc] *= beta;
      
      for(int x = 0; x < n; ++x)
      {
          const double _a = trans ? a[i + x * lda] : a[x + i * lda];
          const double _b = b[j + x * ldb];
          
          
        c[j + i* ldc] += alpha * _a * _b;
      }  */    
      
    }
  }
  
}
