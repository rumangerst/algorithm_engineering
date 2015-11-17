#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define MEMORY_ALIGNMENT 64
#define DISCUSSING true
#define EATING false
#define N 8

/**
 * Returns the status of the philosopher left from philosopher i
 * */
bool left(int i, bool * table)
{
  const bool index = i == 0 ? N - 1 : i - 1;
  return table[index];
}

int main(int argc, char * argv[])
{
  // Monitor the time
  int time = stoi(argv[1]);
  bool time_up = false;
  
  // There are N philosophers at the table
  // They are discussing
  bool table __attribute__((aligned(MEMORY_ALIGNMENT))) [N];
  
  for(int i = 0; i < N; ++i)
    table[i] = DISCUSSING;
  
  // Remember the count of discussions
  int discussions = 0;
  
  // Monitor the time
  auto start = high_resolution_clock::now(); 
  
  #pragma omp parallel
  {
    //Monitor the time
    #pragma omp single nowait
    while(!time_up)
    {
      auto end = high_resolution_clock::now(); 
      auto elapsed_seconds = duration<double>(end-start).count();
      
      if(elapsed_seconds >= time)
      {
	#pragma omp critical
	time_up = true;
      }
    }
    
    //Start the philosophers    
    #pragma omp single
    for(int i = 0; i < N; ++i)
    {
      #pragma omp task shared(discussions, table) firstprivate(i)
      {
	cout << i << endl;
	while(!time_up)
	{
	 
	}
      }
    }
    
    # pragma omp taskwait
  }  
  
  auto end = high_resolution_clock::now(); 
  auto elapsed_seconds = duration<double>(end-start).count();  
  cout << "Runtime: " << elapsed_seconds << "s" << endl;
  
  return EXIT_SUCCESS;
}
