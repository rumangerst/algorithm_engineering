#!/bin/bash

mkdir build
cd build 

CC=mpiicc CXX=mpiicpc cmake ..
#CC=mpicc CXX=mpicxx cmake ..
make
