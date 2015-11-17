// A simple example to try  to get lots of Flops on 
// Intel Xeon Phi co-processors.

#include <cstdlib>

#include <chrono>
#include <iostream>

#define FLOPS_ARRAY_SIZE (1024*1024) 
//#define MAXFLOPS_ITERS 100000000
#define MAXFLOPS_ITERS 1000000
#define LOOP_COUNT 1024*1024
// Floating pt ops per inner loop iteration
// how many FLOPS does every iteration of the inner-loop execute
#define FLOPSPERCALC 2
// define some arrays

// what is the meaning of these attributes?
// Align the floats in the arrays in a 64 byte manner, so each float gets its own cache line
float fa[FLOPS_ARRAY_SIZE] __attribute__((aligned(64)));
float fb[FLOPS_ARRAY_SIZE] __attribute__((aligned(64)));

using namespace std;

int main(int argc, char *argv[] ) {
  // initialize the compute arrays 
  cout << "Initializing" << endl;
  // initialize arrays fa and fb
  
  for(int i = 0; i < FLOPS_ARRAY_SIZE; ++i)
  {
    fa[i] = i;
    fb[i] = i;
  }

  cout << "Starting Compute" << endl;

  auto start = chrono::high_resolution_clock::now();
  
  float constexpr scaling_factor = 1.1f;
  // loop many times to really get lots of calculations
  
  #pragma omp parallel for
  for(int j = 0; j < MAXFLOPS_ITERS; ++j) 
  {
    // scale 1st (fa) array and add in the 2nd (fb) array to the 2nd array
    // this is also known as the *axpy operation within the BLAS library
    for(int k = 0; k < LOOP_COUNT; ++k) 
    {
        // compute axpy as described above on arrays fa and fb using index k
        fb[k] += fa[k] * scaling_factor;       
    }
  }

  // # of gigaflops we just calculated
  // calulate the number of Giga(!)-FLOPS the outer loop just executed
  double gflops = 1.0e-9 * FLOPSPERCALC * MAXFLOPS_ITERS * LOOP_COUNT;

  // elasped time
  auto finish = chrono::high_resolution_clock::now();    	
  auto elapsed_seconds = chrono::duration<double>(finish-start).count();

  // Print the results
  if (elapsed_seconds > 0.0) {
    cout << "GFlops = " << gflops
              << ", Secs = " << elapsed_seconds
              << ", GFlops per sec = " << (gflops / elapsed_seconds)
              << endl;
  }
  exit(EXIT_SUCCESS);
}

