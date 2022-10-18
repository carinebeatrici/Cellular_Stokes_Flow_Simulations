The C Codes to run Vicsek-like active particles simulations need to be
in the scr folder located were the simulation will be compiled and executed.

The configuration files are inside the include folder, all parameters control
are done in compilation time to better execution time. 
Therefore every parameter execution requires recompile and execution.

The simulation configuration file is the param.h 

The results of the simulation are saved on data folder in file posicoes.dat.
This output file presents the following format:

snapshot_time  number_of_particles <br>
x[0] y[0] vx[0] vy[0] box[0] unique_id[0] <br>
x[1] y[1] vx[1] vy[1] box[1] unique_id[1] <br>
... <br>
x[number_of_particles-1] y[number_of_particles-1] vx[number_of_particles-1] vy[number_of_particles-1] box[number_of_particles-1] unique_id[number_of_particles-1] <br>
snapshot_time  number_of_particles <br>
x[0] y[0] vx[0] vy[0] box[0] unique_id[0] <br>
x[1] y[1] vx[1] vy[1] box[1] unique_id[1] <br>
... <br>
x[number_of_particles-1] y[number_of_particles-1] vx[number_of_particles-1] vy[number_of_particles-1] box[number_of_particles-1] unique_id[number_of_particles-1] <br>
