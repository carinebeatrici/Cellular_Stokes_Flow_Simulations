from circle import *
from wall import *
from tissue import *
from sys import stdout

density = 1.0
size_mul = 1.0

wall = Wall(size_mul*226, size_mul*56.5, 1.5*density) # params: Length, Width, number of particles per unit length 
circle = Circle(np.array([size_mul*0.0,0.0,0.0]),1.0*density,size_mul*8.4)  # obstacle centre, number of particles per unit length, radius

wall.make(offset = 0,mask=[1,0,1,1])   # mask means top, left, and bottom wall on, right wall off (no wall)
circle.make(offset = len(wall.pos))

outer_boundary = np.array([[size_mul*-113,size_mul*-28.25],[size_mul*42.0,size_mul*-28.25],[size_mul*42.0,size_mul*28.25],[size_mul*-113,size_mul*28.25]])  # rectangle 

# we make an ellipse for the inner boundary 
phi = np.linspace(0,2*np.pi,32,endpoint=False)
x = size_mul*8.4*np.cos(phi) #  4.0*np.cos(phi) - 1
y = size_mul*8.4*np.sin(phi) #  3.0*np.sin(phi)
inner_boundary = np.vstack((x,y)).T


t = Tissue(outer_boundary, inner_boundary, density)  # last argument in the average bulk density (higher density -> smaller spacing between particles)

t.make_boundary(1.0*density, offset = len(wall.pos) + len(circle.pos))   # makes boundary 

offset = len(wall.pos) + len(circle.pos) + t.Nboundary

np.random.seed()

t.make_stem([size_mul*-112,size_mul*-27.5],[size_mul*-112,size_mul*27.5], stem_density = density,offset = offset)  # make step cells along the line 
t.make_bulk(offset = offset + t.Nstem, max_attempt_factor = 5)   # make cell in the bulk 

# print out input file
with open('tissue.dat','w') as out:
    out.write('keys: id type radius x y z nx ny nz vx vy vz nvx nvy nvz area boundary in_tissue\n')
    out.write('# wall\n')
    for p in wall.pos:
        p.write(out)
    out.write('# obstacle \n')
    for p in circle.pos:
        p.write(out)
    out.write('# tissue\n')
    for p in t.pos:
        p.write(out)

# print out boundary file
with open('boundary.dat','w') as bnd:
    bnd.write('# label id1 id2\n')
    for (i,j,k) in t.boundary_tuples:
        bnd.write('{:3d}  {:5d}  {:5d}\n'.format(i,j,k))


