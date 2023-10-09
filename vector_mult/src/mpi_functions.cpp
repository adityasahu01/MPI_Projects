/*
 *****************************************************
 *  Author - Aditya Sahu
 *  https://github.com/adityasahu01/MPI_Projects
 *  
 *  Copyright (c), 2023
 *****************************************************
 */

#include "mpi_mult_pvt.h"

std::vector<std::vector<int>> mpi_vector_mult(int argc, char** argv,
                                             std::vector<std::vector<int>>& matrix_1,
                                             std::vector<std::vector<int>>& matrix_2) {
    int rows = matrix_1[0].size();
    std::vector<std::vector<int>> result(rows, std::vector<int>(rows, 0));

    int array_a[rows][rows];
    int array_b[rows][rows];
    int array_c[rows][rows];

    memset(array_c, 0, sizeof(array_c));

    for (int idx = 0; idx < rows; ++idx) {
        for (int cols = 0; cols < rows; ++cols) {
            array_a[idx][cols] = matrix_1[idx][cols];
            array_b[idx][cols] = matrix_2[idx][cols];
        }
    }

    int rank;
    int size;
    int from, to, i, j, k;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rows % size != 0) {
        if (rank == 0)
            std::cout << "Matrix size not divisible by the number of processors\n";
        MPI_Finalize();
        exit(-1);
    }

    from = rank * rows / size;
    to = (rank + 1) * rows / size;

    // Create temporary send and receive buffers
    int send_buffer[rows * rows / size];
    int receive_buffer[rows * rows / size];

    // Scatter matrix_a
    MPI_Scatter(array_a, rows * rows / size, MPI_INT, send_buffer, rows * rows / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast matrix_b to all processes
    MPI_Bcast(array_b, rows * rows, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < (to - from); i++) {
        for (j = 0; j < rows; j++) {
            array_c[i][j] = 0;
            for (k = 0; k < rows; k++) {
                array_c[i][j] += send_buffer[i * rows + k] * array_b[k][j];
            }
        }
    }

    // Gather results back to array_c
    MPI_Gather(array_c[from], rows * rows / size, MPI_INT, receive_buffer, rows * rows / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Copy the results back to the result matrix
    if (rank == 0) {
        for (i = 0; i < rows * rows; i++) {
            array_c[i / rows][i % rows] = receive_buffer[i];
        }
    }

    MPI_Finalize();

    for (i = 0; i < rows; i++) {
        for (j = 0; j < rows; j++) {
            result[i][j] = array_c[i][j];
        }
    }

    return result;
}
