#include <matmat.hpp>
#include <iostream>
#include <chrono>
#include <omp.h>
#include <boost/align/aligned_allocator.hpp>
#include <vector>

#define LOOP_COUNT 100

using namespace std;

template <typename T>
using aligned_alloc_64 = boost::alignment::aligned_allocator<T, 64>;

template <typename T>
using aligned_vector = vector<T, aligned_alloc_64<T>>;


int main(int argc, char** argv)
{
  
  if(argc < 2)
  {
    cout << "matmat threads1 threads2 ..." << endl;
    exit(EXIT_FAILURE);
  }
  
  int thread_counts[argc - 1];
  
  for(int i = 1; i < argc; ++i)
    thread_counts[i - 1] = stoi(argv[i]);
  
  for(int N = 64; N < 8192; N *= 2)
  {
    
    for(int t = 0; t < argc - 1; ++t)
    {
      
      for(int trans = 0; trans < 2; ++trans)
      {
      
	int threads = thread_counts[t];
	omp_set_num_threads(threads);
	
	//Actual test matrices
	int m = N;
	int n = N;
	int k = N;
	int lda = N;
	int ldb = N;
	int ldc = N;
	
	aligned_vector<double> A(m * lda);
	aligned_vector<double> B(n * ldb);
	aligned_vector<double> C(m * ldc);
	
	for(int i = 0; i < m * lda; ++i)
	  A[i] = i;
	
	for(int i = 0; i < n * ldb; ++i)
	  B[i] = i - 600;
	
	for(int i = 0; i < m * ldc; ++i)
	  C[i] = 0;
	
	
	auto start = chrono::high_resolution_clock::now(); 
	
	for(int i = 0; i < LOOP_COUNT; ++i)
	{
	  matmat(1.0, trans, A.data(), m, n, lda, B.data(), k, ldb, 1.0, C.data(), ldc);
	}
	
	auto end = chrono::high_resolution_clock::now();
	auto elapsed_seconds = chrono::duration<double>(end-start).count();
	
	cout << "N " << N << " THREADS " << threads << " TRANS " << trans << " RT " << elapsed_seconds << endl;
      }
      
    }
  }  
  
  return 0;
}
