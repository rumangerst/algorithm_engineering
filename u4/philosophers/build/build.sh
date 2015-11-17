#!/bin/bash


CC=icc CXX=icpc cmake -DTARGET_MIC=OFF ..
make
mv philosophers philosophers.no_mic


CC=icc CXX=icpc cmake -DTARGET_MIC=ON ..
make
mv philosophers philosophers.mic
