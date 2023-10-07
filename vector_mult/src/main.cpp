#include "mpi_mult_pvt.h"

void 
usage (void)
{
    std::cout << "Invalid Usage!" << "\n";
    std::cout << "Usage : ./vector_addition <INT : matrix size>" << "\n";
    return;
}

int 
main (int argc, char** argv) 
{
    // Initialize matrices and perform setup
    if (argc != 2) {
        usage();
        return 1;
    }

    int size = 0;

    if (isdigit(*argv[1])) 
    {
        size = atoi(argv[1]);
    } else 
    {
        usage();
        return 1;
    }

    std::vector<std::vector<int>> u = generate_rand_matrix(size);
    std::vector<std::vector<int>> v = generate_rand_matrix(size);

    // Timing for MPI-based matrix addition
    auto start = std::chrono::high_resolution_clock::now();
    
    // Call the MPI-based matrix addition function
    std::vector<std::vector<int>> mpi_res = mpi_vector_mult(argc, argv, u, v);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_mpi = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Timing for regular C++ matrix addition
    start = std::chrono::high_resolution_clock::now();
    // Call the standard C++ matrix addition function

    std::vector<std::vector<int>> std_res = matrix_multiply(u, v);
    end = std::chrono::high_resolution_clock::now();
    
    auto duration_cpp = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Calculate speedup
    double speedup = static_cast<double>(duration_cpp.count()) / duration_mpi.count();

    if (mpi_res != std_res) {
        std::cout << "Outputs don't match" << "\n";
        std::cout << "MPI matrix :" << "\n";
        (void)print_matrix(mpi_res);

        std::cout << "STD matrix :" << "\n";
        (void)print_matrix(std_res);
    }

    

    std::cout << "MPI Execution Time: " << duration_mpi.count() << " ms" << std::endl;
    std::cout << "C++ Execution Time: " << duration_cpp.count() << " ms" << std::endl;
    std::cout << "Speedup: " << speedup << std::endl;

    return 0;
}
