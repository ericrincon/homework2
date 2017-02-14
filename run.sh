#!/bin/bash 

module load papi
git pull
gcc main.cpp -O3 -std=c++11
./main.out