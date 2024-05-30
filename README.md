This is the branch, where I conduct some tests on efficiency of the programm. There are a few scenarios, which will let me compare various versions of the code (stadard, optimized, paralellized)


CODE VARIANTS:
1. Standard: outer loop(timesteps) inner loop(Particles)
2. Standard: inner loop(Particles) outer loop(timesteps)
3. NoSaving: faster variant of the loop; The functionality of saving all the positions is disabled. Instead the statistics will be calculated in real time
4. NoSaving - Parallelized: faster variant of the loop; the code variant with no positions saving; parallelized

SIMULATION CASES:
A. Many particles, short time: end_time =  25; N = 100000*2;
B. Few particles, long time: end_time = 1000; N = 1000*2;
C. Medium particles, medium time: end_time =  100; N = 10000*2;

The saving is diasbled, so the pure speed of the simulation is tested. Saving the particles' positions in each step is really costly!

There will be used formulation of acceleration n.o. 2 (more advanced, more operationss)