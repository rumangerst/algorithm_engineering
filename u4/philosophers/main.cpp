#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

#define MEMORY_ALIGNMENT 64
#define DISCUSSING true
#define EATING false
#define N 8

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
  int time = stoi(argv[1]);
  bool time_up = false;
  
  // There are N philosophers at the table
  // They are discussing
  bool table[N] __attribute__((aligned(MEMORY_ALIGNMENT)));
  
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
        cout << "Time's up" << endl;
      }
    }
    
    //Start the philosophers
    #pragma omp single
    for(int i = 0; i < N; ++i)
    {
      #pragma omp task shared(discussions, table) firstprivate(i)
      {
        // Until time is up the philosopher is alive
        while(!time_up)
        {
            ++discussions;

            // The philosopher is discussing
            cout << "Philososopher "<< i << " discussing" << endl;
            this_thread::sleep_for(seconds(1));

            // Now waiting until there are two forks
            // Then take the forks
            bool can_eat = false;

            //TODO: Use a mutex here!
            while(!can_eat)
            {
                const bool fork_right = table[i];
                const bool fork_left = table[left(i)];

                can_eat = fork_left && fork_right;

                cout << "Philososopher "<< i << " can eat: " << can_eat << endl;
            }

            table[i] = false;
            table[left(i)] = false;

            // The philosopher is eating
            this_thread::sleep_for(seconds(1));

            // Now discuss again
            //TODO: Use a mutex
            table[i] = true;
            table[left(i)] = true;
        }
      }
    }
    
    # pragma omp taskwait
  }
  
  auto end = high_resolution_clock::now(); 
  auto elapsed_seconds = duration<double>(end-start).count();  
  cout << "Runtime: " << elapsed_seconds << "s" << endl;
  cout << "Discussions: " << discussions << endl;
  
  return EXIT_SUCCESS;
}
