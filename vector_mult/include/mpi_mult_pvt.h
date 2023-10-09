/*
 *****************************************************
 *  Author - Aditya Sahu
 *  https://github.com/adityasahu01/MPI_Projects
 *  
 *  Copyright (c), 2023
 *****************************************************
 */

#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <mpi.h>
#include <vector>
#include <cstring>

// void
std::vector<std::vector<int>>
mpi_vector_mult (int, char**, 
                 std::vector<std::vector<int>> &,
                 std::vector<std::vector<int>> &);

// void
std::vector<std::vector<int>>
matrix_multiply (std::vector<std::vector<int>> &, 
                 std::vector<std::vector<int>> &);

std::vector<std::vector<int>>
generate_rand_matrix (int);

void
print_matrix (std::vector<std::vector<int>> &);