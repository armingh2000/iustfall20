# Description
This experiment investigates the possiblity of using multi-processes for
improving the speed of processing.

There is a python script in data directory that will generate 100 files each
having one milion records. The goal is to find the summation of all numbers
in the generated files (in modolus 1e9+7). The answer to the problem is stored
in a file (./data/res.txt) for comparison with the result optain from
implementations.

There are two C programs. `single_proc.c` will go through the files and
calculate the summation. `multi_proc.c` will create 10 children and assign
each child a batch of files to be processed. Then the results are aggregated.

# Running
```
make
./single_proc.o
./multi_proc.o
```

