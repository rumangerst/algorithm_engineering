#!/bin/bash

mkdir build
cd build 

#CC=mpiicc CXX=mpiicpp cmake ..
CC=mpicc CXX=mpicxx cmake ..
make