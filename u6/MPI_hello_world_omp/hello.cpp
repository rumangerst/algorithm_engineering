#include <stdexcept>
#include <iostream>

#include <mpi.h>
#include <omp.h>

using namespace std;

class MPIManager
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
};

int main(int argc, char** argv)
{
    try
    {
        MPIManager mpiMan(argc, argv);
        
	    cout << "hello world" << endl;
        cout << "Comm world size: " << mpiMan.commSize() << ", Comm rank: " << mpiMan.commRank() << endl;
        cout << "Available threads: " << omp_get_max_threads() << endl;
    }
    catch (exception const& e)
    {
        cerr << e.what() << endl;
        return -1;
    }

    return 0;
}

