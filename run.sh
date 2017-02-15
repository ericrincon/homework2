#!/bin/bash 
#SBATCH -J papi           # job name
#SBATCH -o myPAPI.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1              # total number of mpi tasks requested
#SBATCH -p serial     # queue (partition) -- normal, development, etc.
#SBATCH -t 01:30:00        # run time (hh:mm:ss) - 1.5 hours
#SBATCH --mail-user=eric.rincon@utexas.edu
#SBATCH -A Galois
#SBATCH --exclusive
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
module load papi
git pull

icc -L$TACC_PAPI_LIB -lpapi -I$TACC_PAPI_INC -O3 -fp-model precise -o mmmPAPI mmmPAPI.c
echo 50 50
./mmmPAPI 50 50
echo 200 200
./mmmPAPI 200 200
echo 2000 2000
./mmmPAPI 2000 2000

icc -O3 -fp-model precise -o mmmGetTime mmmGetTime.c
echo 50 50
./mmmGetTime 50 50
echo 200 200
./mmmGetTime 200 200
echo 2000 2000
./mmmGetTime 2000 2000