#include "mpi_addition_pvt.h"

// Your matrix addition functions here
// void
std::vector<std::vector<int>>
mpi_vector_add(int argc, char** argv,
               std::vector<std::vector<int>> &matrixA,
               std::vector<std::vector<int>> &matrixB)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int rows = matrixA[0].size();

    // Initialize vectors
    std::vector<std::vector<int>> result(rows, std::vector<int>(rows));


    MPI_Datatype column_type;
    MPI_Type_vector(rows, 1, rows, MPI_INT, &column_type);
    MPI_Type_commit(&column_type);

    std::vector<std::vector<int>> recv_A(matrixA.begin(), matrixA.end());
    std::vector<std::vector<int>> recv_B(matrixB.begin(), matrixB.end());
    std::vector<std::vector<int>> temp(matrixB.begin(), matrixB.end());

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);

    MPI_Scatterv(&matrixA[0][0], &sendcounts[0], &displs[0], column_type, &recv_A[0][0], rows, column_type, 0, MPI_COMM_WORLD);
    MPI_Scatterv(&matrixB[0][0], &sendcounts[0], &displs[0], column_type, &recv_B[0][0], rows, column_type, 0, MPI_COMM_WORLD);

    // Perform local vector addition
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }

    // Gather the result vector C to the root process
    MPI_Gatherv(&temp[0][0], rows, column_type, &result[0][0], &sendcounts[0], &displs[0], column_type, 0, MPI_COMM_WORLD);

    MPI_Type_free(&column_type);
    MPI_Finalize();
    return result;
}
