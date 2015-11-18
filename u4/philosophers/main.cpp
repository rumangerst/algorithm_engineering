#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>

using namespace std;
using namespace std::chrono;

#define MEMORY_ALIGNMENT 64
#define DISCUSSING true
#define EATING false
#define N 10

/**
 * Returns the status of the philosopher left from philosopher i
 * */
bool left(int i)
{
  return i == 0 ? N - 1 : i - 1;
}

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    cout << "philosophers <runtime s>" << endl;
    exit(EXIT_FAILURE);
  }
  
  // Monitor the time
  int time_limit = stoi(argv[1]);
  bool time_up = false;
  
  // There are N philosophers at the table
  // They are discussing
  bool table[N] __attribute__((aligned(MEMORY_ALIGNMENT)));
  
  for(int i = 0; i < N; ++i)
    table[i] = DISCUSSING;
  
  mutex table_mutex;
  
  // Remember the count of discussions
  int discussions = 0;
  
  // Times should be random
  default_random_engine random_generator;
  uniform_int_distribution<int> philosopher_time(1,1000);
  
  // Monitor the time
  auto time_start = high_resolution_clock::now(); 
  
  #pragma omp parallel
  {   
    //Create a time watcher thread
    #pragma omp master
    while(!time_up)
    {
      time_up = duration<double>(high_resolution_clock::now()-time_start).count() >= time_limit;
    }
    
    //Start the philosophers
    #pragma omp single
    for(int i = 0; i < N; ++i)
    {
      #pragma omp task shared(discussions, table, time_up) firstprivate(i)
      {
	
	cout << "Philososopher "<< i << " comes alive" << endl;
	
	// Until time is up the philosopher is alive
	while(!time_up)
	{
	  #pragma omp atomic
	  ++discussions;
	  
	  // The philosopher is discussing
	  //cout << "Philososopher "<< i << " discussing" << endl;
	  this_thread::sleep_for(microseconds(philosopher_time(random_generator)));
	  
	  // Now waiting until there are two forks
	  // Then take the forks
	  bool can_eat = false;	  
	  
	  while(!can_eat && !time_up)
	  {
	    {
	      const bool fork_right = table[i];
	      const bool fork_left = table[left(i)];
	      
	      can_eat = fork_left && fork_right;
	    }    
	    
	    
	    if(can_eat)
	    {
	      //cout << "Philososopher "<< i << " can eat: " << can_eat << endl;
	      
	      //Now lock an wait for access
	      lock_guard<mutex> table_mutex_lock(table_mutex);
	      
	      //cout << "Philososopher "<< i << ": got access" << endl;
	      
	      const bool fork_right = table[i];
	      const bool fork_left = table[left(i)];
	      
	      can_eat = fork_left && fork_right;
	      
	      //If still can eat
	      if(can_eat)
	      {
		//cout << "Philososopher "<< i << ": NOW EATING" << endl;
		table[i] = false;
		table[left(i)] = false;
	      }
	    }
	    else 
	    {
	      //cout << "Philososopher "<< i << " cannot eat" << endl;
	    }
	  }  
	  
	  
	  // The philosopher is eating
	  this_thread::sleep_for(microseconds(philosopher_time(random_generator)));
	  
	  // Now discuss again
	  {
	    //cout << "Philososopher "<< i << ": giving back ..." << endl;
	    
	    lock_guard<mutex> table_mutex_lock(table_mutex);
	    
	    //cout << "Philososopher "<< i << ": giving back ... got lock" << endl;
	    table[i] = true;
	    table[left(i)] = true;
	  }
	    
	}
	
	cout << "Philososopher "<< i << ": ENDING" << endl;
      }
    }
    
    # pragma omp taskwait
  }
  
  auto elapsed_seconds = duration<double>(high_resolution_clock::now() - time_start).count();  
  cout << "Runtime: " << elapsed_seconds << "s" << endl;
  cout << "Discussions: " << discussions << endl;
  
  return EXIT_SUCCESS;
}
