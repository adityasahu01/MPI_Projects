# Install mpich Ubuntu
sudo apt install mpich

## Install CMake
sudo apt install cmake

## Install GTest
sudo apt-get install libgtest-dev

## Build
```bash
cmake .
make
./build/vector_addition <size>
```

** If this code fails on your machine, then it's likely because of the limited stack size
** These arrays are being allocated on the stack, and if they become too large, they can 
** lead to a stack overflow and a segmentation fault. When this happens, try allocating
** memory on the heap using new or malloc instead.