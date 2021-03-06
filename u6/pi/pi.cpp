#include <stdexcept>
#include <iostream>
#include <random>
#include <math.h>

//#include <mpi.h>
#include <omp.h>

using namespace std;

#define R 1

/*class MPIManager
{
public:
    MPIManager(int argc, char** argv)
    {
        if (MPI_SUCCESS != MPI_Init(&argc, &argv ))
            throw std::runtime_error("called MPI_Init twice");
    }
    
    ~MPIManager()
    {
        MPI_Finalize();
    }

    int commSize()
    {
        int s = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &s);

        return s;
    }

    int commRank()
    {
        int r = 0;
        MPI_Comm_rank(MPI_COMM_WORLD, &r);

        return r;
    }
};*/

struct hit_count
{
    ulong Ps = 0;
    ulong Pc = 0;
};

hit_count hit_test(int iterations)
{
    random_device random_device;
    default_random_engine random_generator(random_device());
    uniform_real_distribution<double> random_coordinate(-R, R);
    
    hit_count hits;
    const double radius_squared = R * R;
    
    for(uint i = 0; i < iterations; ++i)
    {
        double x = random_coordinate(random_generator);
        double y = random_coordinate(random_generator);        
        
        if(x*x + y*y <= radius_squared)
            ++hits.Pc;
        else
            ++hits.Ps;
    }
    
    return hits;
} 

void calculate(uint iterations)
{
    const uint available_threads = omp_get_max_threads();
    const uint iterations_per_thread = iterations / available_threads;
    
    ulong Ps = 0;
    ulong Pc = 0;
    
    #pragma omp parallel for default(none) schedule(static,1) reduction(+:Ps,Pc)
    for (uint i = 0; i < available_threads; ++i)
    {
      hit_count partial_hit_count = hit_test(iterations_per_thread);
      
      Ps += partial_hit_count.Ps;
      Pc += partial_hit_count.Pc;
    }
        
    const double pi = 4.0 * Pc / (double)(Ps + Pc);
    
    cout.precision(numeric_limits<double>::max_digits10);    
    cout << "pi " << fixed << pi << endl;
}

int main(int argc, char** argv)
{
    /*try
    {
        MPIManager mpiMan(argc, argv);        
	    calculate(mpiMan);
    }
    catch (exception const& e)
    {
        cerr << e.what() << endl;
        return -1;
    }*/
    
    if(argc != 2)
    {
      cout << "pi <iterations>" << endl;
      return -1;
    }
    
    uint iterations = stoi(argv[1]);
    
    calculate(iterations);

    return 0;
}

