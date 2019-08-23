### Description
Each compared algorithm load these solutions to be the initial solutions. Each dataset has thirty initial solutions because of thirty runs for the comparison.
create_init_sol.cpp is used to generate an initial solution for the algorithm.
init_01.txt ~ init_09.txt are the initial solutions for DS1 ~ DS9, respectively.

### Compile
    g++ create_init_sol.cpp -o create_init_sol.exe -O3

### Usage
Three parameters are required to execute the program.

1. Output filename
2. Number of runs
3. Number of candidate positions

#### For example
Generate "init_example.txt" with 5 runs and 100 candidate positions.

    create_init_sol.exe init_example.txt 5 100