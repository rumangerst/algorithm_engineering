#!/bin/bash

CC=icc CXX=icpc cmake -DTARGET_MIC=OFF ..
make
mv axpy axpy.no_mic

CC=icc CXX=icpc cmake -DTARGET_MIC=ON ..
make
mv axpy axpy.mic
