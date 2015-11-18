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
#define N 4

#define BOOL2_TRUE 257
#define BOOL2_FALSE 0

/**
 * Returns the status of the philosopher left from philosopher i
 * */
bool left(int i)
{
    return i == 0 ? N - 1 : i - 1;
}

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

    //Create a time watcher thread

#pragma omp parallel
    {

    #pragma omp master
    while(!time_up)
    {
        const double dur = duration<double>(high_resolution_clock::now()-time_start).count();

        #pragma omp atomic write
        time_up = dur >= time_limit;
    }

    cout << "starting philosophers"<<endl;

    #pragma omp for schedule(static, 1)
    for(int i = 0; i < N; ++i)
    {
        cout << "Philososopher "<< i << " comes alive" << endl;

        while(!time_up)
        {
            const bool first_item = i == 0;

            #pragma omp atomic update
            ++discussions;

            this_thread::sleep_for(microseconds(philosopher_time(random_generator)));

            bool can_eat = false;

            while(!can_eat && !time_up)
            {
                bool x_left;
                bool x_right;

                if(!first_item)
                {
                    #pragma omp atomic capture
                    {
                        x_left = table[i - 1];
                        table[i - 1] = false;
                    }
                }
                else
                {
                    #pragma omp atomic capture
                    {
                        x_left = table[N - 1];
                        table[N - 1] = false;
                    }
                }

                #pragma omp atomic capture
                {
                    x_right = table[i];
                    table[i] = false;
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
                    table[i] |= x_right;

                    if(!first_item)
                    {
                        #pragma omp atomic update
                        table[i - 1] |= x_left;
                    }
                    else
                    {
                        #pragma omp atomic update
                        table[N - 1] |= x_left;
                    }
                }
            }

            // The philosopher is eating
            this_thread::sleep_for(microseconds(philosopher_time(random_generator)));

            //Put the forks back
            #pragma omp atomic write
            table[i] = true;

            if(!first_item)
            {
                #pragma omp atomic write
                table[i - 1] = true;
            }
            else
            {
                #pragma omp atomic write
                table[N - 1] = true;
            }

            //cout << "philosopher " << i << " finished eating" << endl;
        }

        /*const bool telomeric = i == 0;

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

            if (telomeric)
            {
                //Telomeric case: the both ends need to be locked

                        while(!can_eat && !time_up)
                        {
                            {
                                const bool fork_right = table[i];
                                const bool fork_left = table[left(i)];

                                can_eat = fork_left && fork_right;
                            }


                            if(can_eat)
                            {
                                cout << "Philososopher "<< i << " can eat: " << can_eat << endl;

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
            }
            else
            {
                while(!can_eat && !time_up)
                {
                    // We operate on two bools at the same time ^= unsigned short
                    // To do the atomic operation, reinterpret two adjacent bools into one short
                    unsigned short * forks = reinterpret_cast<unsigned short *>(&table[i - 1]);
                    unsigned short current_forks = 0;

                    // Capture the current state of the two forks and claim the forks
                    #pragma omp atomic capture
                    {
                        current_forks = forks[0];
                        forks[0] = BOOL2_FALSE;
                    }

                    // If the original state is BOOL2(true, true), the philosopher can eat
                    // Otherwise restore
                    if(current_forks == BOOL2_TRUE)
                    {
                        can_eat = true;
                    }
                    else
                    {
                        #pragma omp atomic write
                        *forks = current_forks;
                    }
                }
            }


            // The philosopher is eating
            this_thread::sleep_for(microseconds(philosopher_time(random_generator)));

            // Now discuss again
            if(telomeric)
            {
                cout << "Philososopher "<< i << ": giving back ..." << endl;

                        lock_guard<mutex> table_mutex_lock(table_mutex);

                        cout << "Philososopher "<< i << ": giving back ... got lock" << endl;
                        table[i] = true;
                        table[left(i)] = true;
            }
            else
            {
                cout << "Philososopher "<< i << ": giving back ..." << endl;

                unsigned short * forks = reinterpret_cast<unsigned short *>(&table[i - 1]);

                // Capture the current state of the two forks and claim the forks
                #pragma omp atomic write
                forks[0] = BOOL2_TRUE;
            }

        }

        cout << "Philososopher "<< i << ": ENDING" << endl;

    }*/
    }
    }

    auto elapsed_seconds = duration<double>(high_resolution_clock::now() - time_start).count();
    cout << "Runtime: " << elapsed_seconds << "s" << endl;
    cout << "Discussions: " << discussions << endl;

    return EXIT_SUCCESS;
}
