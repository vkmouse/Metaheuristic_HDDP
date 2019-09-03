## Metaheuristic algorithms for the hyper-dense deployment problem for the 5G environment
This code implements three metaheuristic algorithms, simulated annealing (SA), genetic algorithm (GA), and search economics (SE) respectively, and a greedy algorithm, hill climbing (HC), to solve the hyper-dense deployment problem (HDDP) for the 5G environment.

### Introduction
[Search economics (SE)](https://doi.org/10.1109/SMC.2015.447) is a novel metaheuristic algorithm proposed in 2015. The two main differences between SE and traditional metaheuristic are dividing the search space and investing computation resource depending on the potential of each subspace. The simulated results show that the SE-HDDP[[1]](https://doi.org/10.1016/j.procs.2018.10.161) [[2]](https://doi.org/10.1007/s12652-019-01353-8) is able to find a better solution than the HC, SA, and GA compared in this paper.

### Compile
Install GCC 4.7 or newer version in the [link](https://sourceforge.net/projects/mingw-w64/files/). Execute the command "make" in the path of the Makefile to compile the code.

    make

### Usage
Six parameters are required for all algorithms to execute the program. Each algorithm requires specific parameters for itself.
1. Dataset path
2. Number of iterations
3. Number of evaluations
4. Number of evaluations per output
5. Number of runs
6. Path of the initial solution

#### Hill climbing
No other parameter is required to execute the HC. For example, dataset path is "Dataset/DS2.txt", number of iterations is 10000, number of evaluations is 10000, number of evaluations per output is 1000, number of runs is 2, and path of the initial solution is "init_sol/init_02.txt".

    main.exe hc Dataset/DS2.txt 10000 10000 1000 2 init_sol/init_02.txt

#### Simulated annealing
Four parameters are required to execute the SA.
1. Number of neighbors
2. Initial temperature
3. Minimum temperature
4. Cooling rate

For example, number of neighbors is 7, initial temperature is 10, minimum temperature is 0.01, and cooling rate is 0.98.
 
    main.exe sa Dataset/DS2.txt 10000 10000 1000 2 init_sol/init_02.txt 7 10 0.01 0.98

#### Genetic algorithm
Three parameters are required to execute the GA.
1. Number of chromsomes
2. Crossover rate
3. Mutation rate

For example, number of chromsomes is 20, crossover rate is 0.6, and mutation rate is 0.9.
 
    main.exe ga Dataset/DS2.txt 10000 10000 1000 2 init_sol/init_02.txt 20 0.6 0.9

#### Search economics
Four parameters are required to execute the SE.
1. Number of searchers
2. Number of regions
3. Number of samples
4. Number of players

For example, number of searchers is 4, number of regions is 4, number of samples is 2, and number of players is 3.
 
    main.exe se Dataset/DS2.txt 10000 10000 1000 2 init_sol/init_02.txt 4 4 2 3

