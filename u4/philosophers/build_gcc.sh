#!/bin/bash

EXECUTABLE=philosophers

mkdir build
cd build

cmake -DTARGET_MIC=OFF -DUSE_GCC=ON -DNO_WERROR=ON ..
make
