#!/bin/bash


CC=icc CXX=icpc cmake -DTARGET_MIC=OFF ..
make
mv matmat matmat.no_mic


CC=icc CXX=icpc cmake -DTARGET_MIC=ON ..
make
mv matmat matmat.mic
