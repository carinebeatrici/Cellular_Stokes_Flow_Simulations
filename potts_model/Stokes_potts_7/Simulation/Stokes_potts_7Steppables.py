from PySteppables import *
import CompuCellSetup
import CompuCell
import sys
from PySteppablesExamples import MitosisSteppableBase

from math import *
import random
import numpy as np


multiplicador           = 2.0
cell_div                = 49
lambda_alpha_auto       = 1.0
lambda_alpha_collective = 5.0
lambda_inertia          = 0.0
alpha                   = 100.0

center_x                = multiplicador * 569

lambda_v      = 10.0
target_v      = multiplicador * multiplicador * 49 

save_positions = 20
save_positions_after = 0

division_freq = 100


class ConstraintInitializerSteppable(SteppableBasePy):
    def __init__(self,_simulator,_frequency=1):
        SteppableBasePy.__init__(self,_simulator,_frequency)
    def start(self):
        for cell in self.cellList:
            cell.targetVolume = target_v
            cell.lambdaVolume = lambda_v


class MitosisSteppable(MitosisSteppableBase):
    def __init__(self,_simulator,_frequency=1):
        MitosisSteppableBase.__init__(self,_simulator, _frequency)
    def step(self,mcs):
        # print "INSIDE MITOSIS STEPPABLE"
        cells_to_divide=[]
        contador = 0
        for cell in self.cellList:  
            if mcs%division_freq == 0:
                if cell.xCOM > 2*center_x - 2*sqrt(target_v) and cell.xCOM < 2*center_x - sqrt(target_v):
                    contador = contador + 1
            if cell.volume > (multiplicador * multiplicador * cell_div) and cell.xCOM < (multiplicador * 20):
                if cell.type == 1:
                    cells_to_divide.append(cell)
            if cell.type==1 and cell.xCOM > 2*center_x:
                cell.targetVolume = 0
                cell.lambdaVolume = 100
        for cell in cells_to_divide:
            self.divideCellRandomOrientation(cell)
        if mcs%100 == 0:    
            print mcs, contador
    def updateAttributes(self):
        #self.parentCell.targetVolume /= 2.0 # reducing parent target volume                 
        self.cloneParent2Child()            
        


class DeathSteppable(SteppableBasePy):
    def __init__(self,_simulator,_frequency=1):
        SteppableBasePy.__init__(self,_simulator,_frequency)
    def step(self,mcs):
        for cell in self.cellList:
            print cell.xCOM    
            if cell.type==1 and cell.yCOM > 100:# 2*center_x:
            #if cell.xCOM > 30:# 2*center_x:
                #print cell.xCOM
                cell.targetVolume=0
                cell.lambdaVolume=100


class GrowthSteppable(SteppableBasePy):
    def __init__(self,_simulator,_frequency=1):
        SteppableBasePy.__init__(self,_simulator,_frequency)
    def step(self,mcs):
        for cell in self.cellList:
            if cell.type==1:
                if cell.xCOM < (multiplicador * 10):
                    cell.targetVolume+=2
                else:
                    cell.targetVolume = target_v
#            if cell.type==1 and cell.yCOM > 100:# 2*center_x:
#                print cell.xCOM
#                cell.targetVolume=0
#                cell.lambdaVolume=100
    

class SavePositions(SteppableBasePy):
    n = 0
    def __init__(self, _simulator, _frequency=1):
        SteppableBasePy.__init__(self, _simulator, _frequency)
    def start(self):
        screenDir = "./" 
#        screenDir = SteppableBasePy.getScreenshotDirectoryName()
#        screenDir="/home/carine/pesq/stokes-potts/"
        screenDir = CompuCellSetup.getScreenshotDirectoryName() # Defines the output directory for the files.
        print " ************************* DIRETORIO: ", screenDir
        self.output_posicoes = open(screenDir+'/'+'posicoes.dat','w+')
##       self.output_neighbor = open(screenDir+'/'+'neighbor.dat','w+')
        print "****************************************************"
        print "File path:",  self.output_posicoes
    def step(self, mcs):
        if mcs%save_positions == 0 and mcs >= save_positions_after:
            n = 0
            for cell in self.cellListByType(1):
                n = n + 1
            self.output_posicoes.write('%d %d %s \n' % (mcs, n, "*****"))
##            self.output_neighbor.write('%d %d %s \n' % (mcs, n, "*****"))
            for cell in self.cellListByType(1):
##                cellNeighborList = self.getCellNeighbors(cell)
                dx = cell.xCOM - cell.xCOMPrev 
                dy = cell.yCOM - cell.yCOMPrev
                dz = cell.zCOM - cell.zCOMPrev
                self.output_posicoes.write('%f %f %f %f %f %f %d\n' % (cell.xCOM, cell.yCOM, dx, dy, cos(cell.dict['angle']), sin(cell.dict['angle']), cell.id))
##                cellNeighborList = self.getCellNeighbors(cell)
##                for neighbor in cellNeighborList:
##                    if neighbor.neighborAddress:
##                        if neighbor.neighborAddress.type == 1:
##                            self.output_neighbor.write("%d " % neighbor.neighborAddress.id)
##                self.output_neighbor.write(" \n")    
#                print >>fileHandle, cell.id, cell.xCOM, cell.yCOM, dx, dy, cellNeighborList


class CellMotilitySteppable(SteppableBasePy):
    def __init__(self, _simulator, _frequency=1):
        SteppableBasePy.__init__(self, _simulator, _frequency)
        self.track_cell_level_scalar_attribute(field_name='ANGLE', attribute_name='angle')
        #self.pixelTrackerPlugin()
        self.vectorField = self.createVectorFieldCellLevelPy("VectorField") 
    def start(self):
        self.vectorField.clear()
        for cell in self.cellList:    
            if cell.type == 1:
                random_angle = 6.283185 * random.random() - 3.14159
                cell.lambdaVecX = cos(random_angle)
                cell.lambdaVecY = sin(random_angle)
                cell.dict['angle'] = random_angle #atan2(cell.lambdaVecY,cell.lambdaVecX)
                self.vectorField[cell] = [-cos(random_angle), -sin(random_angle), 0.0]
#                print self.vectorField[cell],cell.dict['angle']
            else:
                cell.dict['angle'] = -1.35        
    def step(self, mcs):        
        self.vectorField.clear()
        if mcs > 1:
            for cell in self.cellList:
                displ_x = cell.xCOM - cell.xCOMPrev
                displ_y = cell.yCOM - cell.yCOMPrev
                displ_z = cell.zCOM - cell.zCOMPrev     
                displacement = [displ_x, displ_y, displ_z, sqrt(displ_x * displ_x + displ_y * displ_y + displ_z * displ_z)]
                if displacement[3] == 0.0:
                    displacement[3] = 1.0
                align_x = 0.0; align_y = 0.0; align_z = 0.0;
                cellNeighborList = self.getCellNeighbors(cell)
                for neighbor in cellNeighborList:
                    if neighbor.neighborAddress:
                        if neighbor.neighborAddress.type == 1:
#                            print neighbor.neighborAddress.dict['angle']
                            dx = cos(neighbor.neighborAddress.dict['angle'])
                            dy = sin(neighbor.neighborAddress.dict['angle'])
                            dz = 0.0
                            dr = sqrt(dx * dx + dy * dy + dz * dz)
                            if dx != 0.0 or dy !=  0.0  or dz != 0.0:
                                align_x += dx/dr
                                align_y += dy/dr
                                align_z += dz/dr
                align_r = sqrt(align_x*align_x + align_y*align_y + align_z*align_z)
                if align_r == 0.0:
                    align_r = 1.0;
                collective = [align_x, align_y, align_z, align_r]

                alpha_x = - lambda_inertia * cos(cell.dict['angle']) - lambda_alpha_collective * collective[0]/collective[3] - lambda_alpha_auto * displacement[0]/displacement[3]
                alpha_y = - lambda_inertia * sin(cell.dict['angle']) - lambda_alpha_collective * collective[1]/collective[3] - lambda_alpha_auto * displacement[1]/displacement[3]
                alpha_z = 0.0
            
                alpha_r = sqrt(alpha_x*alpha_x + alpha_y*alpha_y + alpha_z*alpha_z)
            
                if alpha_r == 0:
                    alpha_r = 1.0
                
                cell.lambdaVecX = alpha * alpha_x/alpha_r  # force component along X axis
                cell.lambdaVecY = alpha * alpha_y/alpha_r  # force component along Y axis
                cell.lambdaVecZ = 0.0
                self.vectorField[cell] = [displacement[0], displacement[1], displacement[2]]
                cell.dict['angle'] = atan2(-alpha_y,-alpha_x) #(-cell.lambdaVecX, -cell.lambdaVecY) #(displacement[1], displacement[0])
#                if cell.type == 1:
#                    print self.vectorField[cell],cell.dict['angle']
                if cell.type == 2:
                    cell.dict['angle'] = -1.35
        else:
            for cell in self.cellList:
                cell.lambdaVecX = alpha * -1.0 
                cell.lambdaVecY = alpha * 0.0 
                self.vectorField[cell] = [-cell.lambdaVecX, -cell.lambdaVecY, 0.0]



 





class save_snapshot(SteppableBasePy):
    def __init__(self, _simulator, _frequency=1):
        SteppableBasePy.__init__(self, _simulator, _frequency)
        self.track_cell_level_scalar_attribute(field_name='ANGLE', attribute_name='angle')
        self.vectorField = self.createVectorFieldCellLevelPy("VectorField")
        #x_size = self.dim.x
        #y_size = self.dim.y
        #z_size = self.dim.z  
    def start(self):
        self.screenDir = "./" 
        self.screenDir = CompuCellSetup.getScreenshotDirectoryName()
        #image_matrix = np.zeros(x_size, y_size, z_size, 3)
    def step(self, mcs):  
        if mcs%save_positions == 0 and mcs >= save_positions_after:
            self.output_image = open(self.screenDir+'/'+'img_'+"%06d"%mcs+'.txt','w+')
            print " ************************* DIRETORIO: ", self.screenDir
            print "File path:",  self.output_image
            for x,y,z in self.everyPixel():
                cell = self.cell_field[x,y,z]
                vector = self.vectorField[cell]
                angle = atan2(vector[1],vector[0])
                #angle = (angle/6.2830) + 0.5 
                #rgb_color = colorsys.hsv_to_rgb(angle,0.5,0.5)
                #image_matrix[x][y] = rgb_color
                self.output_image.write(str(x)+'  '+str(y)+'  ',str(angle)+ '\n')
            self.output_image.close()