#!/bin/bash

EXECUTABLE=philosophers

mkdir build
cd build

CC=icc CXX=icpc cmake -DTARGET_MIC=OFF ..
make
mv $EXECUTABLE "$EXECUTABLE.no_mic"

CC=icc CXX=icpc cmake -DTARGET_MIC=ON ..
make
mv $EXECUTABLE "$EXECUTABLE.mic"

