#!/bin/bash

PROGRAM=hello
DIRECTORY=/home/ruman/algorithm_engineering/u6/MPI_hello_world_omp

mpirun -machinefile $DIRECTORY/machine_file $DIRECTORY/build/$PROGRAM
