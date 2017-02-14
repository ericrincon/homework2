#!/bin/bash 
#SBATCH -J papi           # job name
#SBATCH -o myPAPI.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1              # total number of mpi tasks requested
#SBATCH -p normal     # queue (partition) -- normal, development, etc.
#SBATCH -t 01:30:00        # run time (hh:mm:ss) - 1.5 hours
#SBATCH --mail-user=eric.rincon@utexas.edu
#SBATCH -A Galois
#SBATCH --exclusive
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
module load papi
module load gcc
git pull
gcc -c main.cpp -O3 -std=gnu++0x $PAPI_CFLAGS
gcc -o main main.o $PAPI_LIB

./main.out