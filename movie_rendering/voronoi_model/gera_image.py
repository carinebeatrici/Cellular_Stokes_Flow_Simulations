from scipy.spatial import Voronoi, voronoi_plot_2d
import math
import matplotlib as plt
import numpy as np
import os

max_number_particles = 0
os.system("ls cell_*.dat > files.dat")
file_names = open("files.dat",'r')
counter = 0

for line in file_names:
    counter      += 1
    part_counter  = 0
    for word in line.split():
        name_arq_data_in = word
        print name_arq_data_in
        file_arq_data_in = open(name_arq_data_in)
        while 1:
            line = file_arq_data_in.readline()
#            print line
            if not line:
                break #EOF
            line_splitted = line.split()
            if line_splitted[1] == '1' :
                part_counter += 1
                max_number_particles  = max(max_number_particles,part_counter)
                #print part_counter
        file_arq_data_in.close()

print "fim da primeira leitura"                
#file_names.close                
os.system("ls cell_*.dat > files.dat")
file_names = open("files.dat",'r')
image = 0
for datafile in file_names:
    points = []
    speeds = []
    p_type = []
    name_arq_data_in = datafile.split()[0]
    file_arq_data_in = open(name_arq_data_in)
    print "Reading image:",image
    line   = file_arq_data_in.readline()
    while 1 :
        line   = file_arq_data_in.readline()
        if not line:
            image          += 1
            break #EOF
        line_splitted       = line.split()
#        print line_splitted
        particle_type, x, y = int(line_splitted[1]), float(line_splitted[2]), float(line_splitted[3])
        z, vx, vy           = float(line_splitted[4]), float(line_splitted[5]), float(line_splitted[6])
        
        p_type.append([particle_type])
        speeds.append([vx,vy])
        points.append([x,y])
    number_particles = len(points)     
    vor = Voronoi(points)
    # criar arquivo vor#cont.pov
    string_image     = "%04d" % image
    nome_arquivo_vor = "vor" + string_image + ".pov"
    nome_arquivo_vor
    
    
    file_arquivo_vor = open(nome_arquivo_vor,"w")
    file_arquivo_vor.write("#include \"colors.inc\" \n")

    print file_arquivo_vor
    
    obst_x   = 0.000
    obst_y   = 0.000
    obst_r   = 8.4
    
    camera_x = 0.000
    camera_y = 0.000
    camera_z = 10*obst_r
    
    look_x   = obst_x
    look_y   = obst_y
    look_z   = 0.000
    
    x_max =  113
    x_min = -113

    y_max =  28.25
    y_min = -28.25
    
    particle_size = 0.333
    
    file_arquivo_vor.write("camera{location<"+str(camera_y)+", "+str(camera_z)+", "+str(camera_x)+
                            ">look_at<"+str(look_y)+","+str(look_z)+","+str(look_x)+">} \n")
    file_arquivo_vor.write("light_source{<"+str(obst_y)+", "+str(camera_z)+", "+str(obst_x)+"> color White} \n")
    
    file_arquivo_vor.write("plane { y, 0.0 pigment{color rgb<0.0, 0.0, 0.0>}} \n")    
    file_arquivo_vor.write("disc{< 0.0, "+str(1.0*particle_size)+", 0.0 >, < 0.0, 1.0, 0.0 >, "+str(obst_r)+", pigment{ color rgb<0.5, 0.5, 0.5 > } } \n")
    
    num_particles = len(p_type)
    print(num_particles)
    for i in range(num_particles):
        x  = points[i][0]
        y  = points[i][1]
        vx = speeds[i][0]
        vy = speeds[i][1]
        tp = p_type[i][0]
        
        if ((x-obst_x)*(x-obst_x)+(y-obst_y)*(y-obst_y) >=  obst_r*obst_r):
            angle = math.atan2(vy,vx)
            angle = int((angle/6.2832) * 360)
            if angle < 0:
                angle = angle + 360
                angle = int(angle)

            if tp == 1:
                file_arquivo_vor.write("sphere{<"+str(x)+", "+str(particle_size)+", "+str(y)+" >,"+str(particle_size)+" pigment{color CHSL2RGB(<"+str(angle)+", 1.0, 0.5>)} finish{phong 1 } } \n")
            #else:
            #    file_arquivo_vor.write("sphere{<"+str(x)+", "+str(particle_size)+", "+str(y)+" >,"+str(particle_size)+" pigment{color White} } \n")
    
    num_lines = len(vor.ridge_vertices)
    for i in range(num_lines):
        if vor.ridge_vertices[i][0] >= 0 and vor.ridge_vertices[i][0] >= 0:
            x1 = vor.vertices[vor.ridge_vertices[i][0]][0]
            y1 = vor.vertices[vor.ridge_vertices[i][0]][1]
            x2 = vor.vertices[vor.ridge_vertices[i][1]][0]
            y2 = vor.vertices[vor.ridge_vertices[i][1]][1]
            if ((x1-obst_x)*(x1-obst_x)+(y1-obst_y)*(y1-obst_y) >=  obst_r*obst_r) and ((x2-obst_x)*(x2-obst_x)+(y2-obst_y)*(y2-obst_y) >=  obst_r*obst_r):
                if x1 >= x_min and x1 <= x_max and x2 >= x_min and x2 <= x_max and y1 >= y_min and y1 <= y_max and y2 >= y_min and y2 <= y_max:
                    file_arquivo_vor.write("cylinder{<"+str(x1)+", 0.5,"+str(y1)+" >, <"+str(x2)+", 0.5,"+str(y2)+"> 0.1 pigment { color White } } \n")
            
            
#    file_arquivo_vor.write(" \n")






