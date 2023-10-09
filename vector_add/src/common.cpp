/*
 *****************************************************
 *  Author - Aditya Sahu
 *  https://github.com/adityasahu01/MPI_Projects
 *  
 *  Copyright (c), 2023
 *****************************************************
 */

#include "mpi_addition_pvt.h"

// Print Matrix
void
print_matrix(std::vector<std::vector<int>> &v)
{
    int rows = v.size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            std::cout << v[i][j] << "\t";
        }
    }
    std::cout << "\n";
}

// Generate random matrix
std::vector<std::vector<int>> 
generate_rand_matrix(int rows) 
{
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(rows));

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = std::rand() % 100 + 1;
        }
    }

    return matrix;
}

// void
std::vector<std::vector<int>>
matrix_addition(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B) 
{
    int rows = A.size();
    std::vector<std::vector<int>> result(rows, std::vector<int>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}