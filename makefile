#module load mpich/ge/gcc/64/3.2.1
ring: parallelSum.c
	mpicc -o $@ parallelSum.c

clean: 
	rm parallelSum log_slurm.*
