#include "mpi_mult_pvt.h"

// Print Matrix
void
print_matrix (std::vector<std::vector<int>> &v)
{
    int rows = v.size();
    std::cout << "Result of Vector Addition:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            std::cout << v[i][j] << "\t";
        }
    }
    std::cout << "\n";
}

// Generate random matrix
std::vector<std::vector<int>> 
generate_rand_matrix (int rows) 
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

std::vector<std::vector<int>>
matrix_multiply (std::vector<std::vector<int>> &matrix_1, 
                 std::vector<std::vector<int>> &matrix_2) 
{
    size_t size = matrix_1.size();
    std::vector<std::vector<int>> result(size, std::vector<int>(size));

    for (int i = 0; i < (int)size; ++i) {
        for (int j = 0; j < (int)size; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < (int)size; ++k) {
                result[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }

    return result;
}