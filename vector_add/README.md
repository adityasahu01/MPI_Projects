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

**Makefile in /temp if you want to build test binaries**