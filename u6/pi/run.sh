#!/bin/bash

PROGRAM=pi
DIRECTORY=/home/ruman/algorithm_engineering/u6/pi

mpirun -machinefile $DIRECTORY/machine_file $DIRECTORY/build/$PROGRAM
