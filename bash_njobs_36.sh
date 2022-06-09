#!/bin/bash 

#SBATCH --nodes=4

# set the number of tasks (processes) per node.
#SBATCH --ntasks-per-node=9

#SBATCH --partition=all
# set max wallclock time
#SBATCH --time=15:00:00

# set name of job
#SBATCH --job-name=rp-rvp-36

p=36
for size in 840 2520; do
	mpirun -np $p out "${PWD}/matrix/A${size}.txt"\
		            	"${PWD}/matrix/B${size}.txt"\
				"${PWD}/matrix/C${size}.txt"\
				"${PWD}/matrix/C${size}($p proc).txt"\
				"${PWD}/bench.txt" </dev/null
done

