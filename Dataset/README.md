### Description
CreateDataset.cpp is used to generate datasets.
DS1 ~ DS9 are the datasets with different parameters.

Dataset         | region length | region width | number of candidate positions | number of users
--------------- |:-------------:|:------------:|:-----------------------------:|:---------------:
DS1             | 100           | 100          | 50                            | 1,000
DS2             |               |              | 100                           | 1,000
DS3             |               |              | 100                           | 2,000
DS4             | 200           | 200          | 100                           | 2,000
DS5             |               |              | 200                           | 2,000
DS6             |               |              | 200                           | 4,000
DS7             | 300           | 300          | 200                           | 4,000
DS8             |               |              | 300                           | 4,000
DS9             |               |              | 300                           | 8,000

### Compile
    g++ CreateDataset.cpp -o CreateDataset.exe -O3

### Usage
Six parameters are required to execute the program.

1. Output filename
2. Region length
3. Region width
4. Number of candidate positions
5. Number of users
6. Number of existing macrocells

#### For example
Generate a dataset with filename "Dataset.txt", region length is 10, region width is 20, the number of candidate positions is 15, the number of users is 100, and without any existing macrocells.

    CreateDataset.exe Dataset.txt 10 20 15 100 0
