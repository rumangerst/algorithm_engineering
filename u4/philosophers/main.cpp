#include <iostream>
#include <chrono>
//#include <thread>
//***** intel compiler does not support complete stl
//Also cannot use boost::...::sleep_for because mic does not support it
#include <unistd.h>
#include <mutex>
#include <random>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define MEMORY_ALIGNMENT 64
#define DISCUSSING true
#define EATING false
#define N 20

#define DISCUSSION_EATING_TIME 1,10


void print_table(bool table[])
{
    for(int i = 0; i < N; ++i)
    {
        cout << table[i];
    }

    cout << endl;
}

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        cout << "philosophers <runtime s>" << endl;
        exit(EXIT_FAILURE);
    }
    
    omp_set_num_threads(N + 1); // 1 master + N philosophers

    // Monitor the time
    int time_limit __attribute__((aligned(MEMORY_ALIGNMENT))) = stoi(argv[1]);
    bool time_up __attribute__((aligned(MEMORY_ALIGNMENT))) = false;

    // There are N philosophers at the table
    // They are discussing
    bool table[N * 8] __attribute__((aligned(MEMORY_ALIGNMENT)));

    for(int i = 0; i < N; ++i)
        table[i * 8] = DISCUSSING;


    // Remember the count of discussions
    int discussions __attribute__((aligned(MEMORY_ALIGNMENT))) = 0;    

    // Monitor the time
    const high_resolution_clock::time_point time_start = high_resolution_clock::now();

    //Create a time watcher thread

    #pragma omp parallel shared(time_up)
    {

    #pragma omp master
    while(!time_up)
    {
        const double dur = duration<double>(high_resolution_clock::now()-time_start).count();

	if(dur >= time_limit)
	{
            //#pragma omp atomic write
            // Dont know why but leads to effect that this variable is not set!
            time_up = true;
        }       
        
    }

    //cout << "starting philosophers"<<endl;

    #pragma omp for schedule(static, 1)
    for(int i = 0; i < N; ++i)
    {
        //cout << "Philososopher "<< i << " comes alive" << endl;
        
        const int left_fork = i == 0 ? N - 1 : i - 1;

        // Times should be random
        // 1 object per thread prevents need of locks
        default_random_engine random_generator __attribute__((aligned(MEMORY_ALIGNMENT)));
        uniform_int_distribution<int> philosopher_time(DISCUSSION_EATING_TIME);

        while(!time_up)
        {        
            #pragma omp atomic update
            ++discussions;

            //this_thread::sleep_for(microseconds(philosopher_time(random_generator)));
            //boost::this_thread::sleep_for(boost::chrono::microseconds(philosopher_time(random_generator)));
            usleep(philosopher_time(random_generator));

            bool can_eat = false;

            while(!can_eat && !time_up)
            {
                bool x_left;
                bool x_right;
                
		#pragma omp atomic capture
		{
		    x_left = table[left_fork * 8];
		    table[left_fork * 8] = false;
		}

                #pragma omp atomic capture
                {
                    x_right = table[i * 8];
                    table[i * 8] = false;
                }

                if(x_left && x_right)
                {
                    // Both captured values are true and the forks have been claimed -> eat
                    can_eat = true;
                    //cout << i << ":";
                    //print_table(table);
                }
                else
                {
                    //Put forks back from memory
                    //To be sure do not replace already existing forks

                    #pragma omp atomic update
                    table[i * 8] |= x_right;
                   
                    #pragma omp atomic update
                    table[left_fork * 8] |= x_left;                   
                       
                }
            }

            // The philosopher is eating
            //this_thread::sleep_for(microseconds(philosopher_time(random_generator)));
            //boost::this_thread::sleep_for(boost::chrono::microseconds(philosopher_time(random_generator)));
            usleep(philosopher_time(random_generator));

            //Put the forks back
            #pragma omp atomic write
            table[i * 8] = true;

            #pragma omp atomic write
            table[left_fork * 8] = true;

            //cout << "philosopher " << i << " finished eating" << endl;
        }      
        
        //cout << "philosopher " << i << "ended."  << endl;
    }
    }

    auto elapsed_seconds = duration<double>(high_resolution_clock::now() - time_start).count();
    cout << "Runtime: " << elapsed_seconds << "s" << endl;
    cout << "Discussions: " << discussions << endl;

    return EXIT_SUCCESS;
}
