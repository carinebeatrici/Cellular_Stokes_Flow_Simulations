def configureSimulation(sim):
    import CompuCellSetup    
    from XMLUtils import ElementCC3D
    
    
    multiplicador = 2.0
    size_x        = multiplicador * 1500
    size_y        = multiplicador * 292
    size_z        = 1
 
    size_wall     = multiplicador * 5
    size_obst     = multiplicador * 42.3
    
    
    center_x      = multiplicador * 569
    center_y      = multiplicador * 146
    
    lambda_v      = 10.0
    target_v      = multiplicador * multiplicador * 49
    lambda_s      = 2.0
    p_0           = 5.0 # 3.55
    target_s      = p_0 * (target_v ** 0.5)
    
    size_init     = target_v ** 0.5
    
    J             = 150.0
    
    #l_canto       = 2.0 * size_wall + multiplicador * 5
    l_canto       = 2 * center_x #- size_obst
    size_y_w      = size_y - size_wall
    
    CompuCell3DElmnt = ElementCC3D("CompuCell3D",{"Revision":"20180802","Version":"3.7.8"})
    PottsElmnt       = CompuCell3DElmnt.ElementCC3D("Potts")
    
    # Basic properties of CPM (GGH) algorithm
    PottsElmnt.ElementCC3D("Dimensions",{"x":size_x,"y":size_y,"z":size_z})
    PottsElmnt.ElementCC3D("Steps",{},"2020")
    PottsElmnt.ElementCC3D("Temperature",{},"50.0")
    PottsElmnt.ElementCC3D("NeighborOrder",{},"1")
    
    PluginElmnt      = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"CellType"})
    
    # Listing all cell types in the simulation
    PluginElmnt.ElementCC3D("CellType",{"TypeId":"0","TypeName":"Medium"})
    PluginElmnt.ElementCC3D("CellType",{"TypeId":"1","TypeName":"Cell"})
    PluginElmnt.ElementCC3D("CellType",{"Freeze":"","TypeId":"2","TypeName":"Wall"})
    
    
    PluginElmnt_1    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"Volume"})
#    PluginElmnt_1.ElementCC3D("VolumeEnergyParameters",{"CellType":"Cell","LambdaVolume":lambda_v,"TargetVolume": target_v})
    
#    PluginElmnt_2    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"Surface"})
#    PluginElmnt_2.ElementCC3D("SurfaceEnergyParameters",{"CellType":"Cell","LambdaSurface":lambda_s,"TargetSurface":target_s})
 
    
    PluginElmnt_3    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"ExternalPotential"})
    PluginElmnt_3.ElementCC3D("Algorithm",{},"PixelBased")

    
    PluginElmnt_4    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"CenterOfMass"})
    PluginElmnt_5    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"NeighborTracker"})
    
    
    PluginElmnt_6    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"Contact"})
    # Specification of adhesion energies
    PluginElmnt_6.ElementCC3D("Energy",{"Type1":"Medium","Type2":"Medium"},"0.0")
    PluginElmnt_6.ElementCC3D("Energy",{"Type1":"Medium","Type2":"Cell"}  ,J)
    PluginElmnt_6.ElementCC3D("Energy",{"Type1":"Medium","Type2":"Wall"}  ,J)
    PluginElmnt_6.ElementCC3D("Energy",{"Type1":"Cell","Type2":"Cell"}    ,J)
    PluginElmnt_6.ElementCC3D("Energy",{"Type1":"Cell","Type2":"Wall"}    ,J)
    PluginElmnt_6.ElementCC3D("Energy",{"Type1":"Wall","Type2":"Wall"}    ,J)
    PluginElmnt_6.ElementCC3D("NeighborOrder",{},"4")
    
    
    #PluginElmnt_8    = CompuCell3DElmnt.ElementCC3D("Plugin",{"Name":"Connectivity"})
    # self.connectivityLocalFlexPlugin.setConnectivityStrength(cell,10000000)
    #PluginElmnt_8.ElementCC3D("Penalty",{},"100000")
    
    SteppableElmnt   = CompuCell3DElmnt.ElementCC3D("Steppable",{"Type":"UniformInitializer"})

    RegionElmnt      = SteppableElmnt.ElementCC3D("Region")
    RegionElmnt.ElementCC3D("BoxMin",{"x":size_wall,"y":size_wall,"z":"0"})
    RegionElmnt.ElementCC3D("BoxMax",{"x":l_canto,"y":size_y_w,"z":size_z})
    RegionElmnt.ElementCC3D("Gap",{},"0")
    RegionElmnt.ElementCC3D("Width",{},size_init)
    RegionElmnt.ElementCC3D("Types",{},"Cell")


    RegionElmnt      = SteppableElmnt.ElementCC3D("Region")
    RegionElmnt.ElementCC3D("BoxMin",{"x":"0","y":"0","z":"0"})
    RegionElmnt.ElementCC3D("BoxMax",{"x":size_x,"y":size_wall,"z":size_z})
    RegionElmnt.ElementCC3D("Gap",{},"0")
    RegionElmnt.ElementCC3D("Width",{},size_init)
    RegionElmnt.ElementCC3D("Types",{},"Wall")
    RegionElmnt      = SteppableElmnt.ElementCC3D("Region")
    RegionElmnt.ElementCC3D("BoxMin",{"x":"0","y":size_y_w,"z":"0"})
    RegionElmnt.ElementCC3D("BoxMax",{"x":size_x,"y":size_y,"z":size_z})
    RegionElmnt.ElementCC3D("Gap",{},"0")
    RegionElmnt.ElementCC3D("Width",{},size_init)
    RegionElmnt.ElementCC3D("Types",{},"Wall")
    RegionElmnt      = SteppableElmnt.ElementCC3D("Region")
    RegionElmnt.ElementCC3D("BoxMin",{"x":"0","y":size_wall,"z":"0"})
    RegionElmnt.ElementCC3D("BoxMax",{"x":size_wall,"y":size_y_w,"z":size_z})
    RegionElmnt.ElementCC3D("Gap",{},"0")
    RegionElmnt.ElementCC3D("Width",{},size_init)
    RegionElmnt.ElementCC3D("Types",{},"Wall")
    
    
    SteppableElmnt   = CompuCell3DElmnt.ElementCC3D("Steppable",{"Type":"BlobInitializer"})
    RegionElmnt      = SteppableElmnt.ElementCC3D("Region")
    RegionElmnt.ElementCC3D("Center",{"x":center_x,"y":center_y,"z":"0"})
    RegionElmnt.ElementCC3D("Radius",{},size_obst)
    RegionElmnt.ElementCC3D("Gap",{},"0")
    RegionElmnt.ElementCC3D("Width",{},"2")
    RegionElmnt.ElementCC3D("Types",{},"Wall")


    CompuCellSetup.setSimulationXMLDescription(CompuCell3DElmnt)
    
#     CompuCellSetup.setSimulationXMLDescription(CompuCell3DElmnt)
            
import sys
from os import environ
from os import getcwd
import string

sys.path.append(environ["PYTHON_MODULE_PATH"])


import CompuCellSetup


sim,simthread = CompuCellSetup.getCoreSimulationObjects()

#pyAttributeAdder,listAdder=CompuCellSetup.attachDictionaryToCells(sim)
        
configureSimulation(sim)            
            
# add extra attributes here
        
CompuCellSetup.initializeSimulationObjects(sim,simthread)
# Definitions of additional Python-managed fields go here
        
#Add Python steppables here
steppableRegistry=CompuCellSetup.getSteppableRegistry()
 
 
from Stokes_potts_7Steppables import ConstraintInitializerSteppable
ConstraintInitializerSteppableInstance=ConstraintInitializerSteppable(sim,_frequency=1)
steppableRegistry.registerSteppable(ConstraintInitializerSteppableInstance)

from Stokes_potts_7Steppables import MitosisSteppable
steppableInstance=MitosisSteppable(sim,_frequency=1)
steppableRegistry.registerSteppable(steppableInstance)
  
from Stokes_potts_7Steppables import SavePositions
steppableInstance = SavePositions(sim,_frequency=1)
steppableRegistry.registerSteppable(steppableInstance)

from Stokes_potts_7Steppables import CellMotilitySteppable
steppableInstance=CellMotilitySteppable(sim,_frequency=1)
steppableRegistry.registerSteppable(steppableInstance)

from Stokes_potts_7Steppables import save_snapshot
steppableInstance=save_snapshot(sim,_frequency=1)
steppableRegistry.registerSteppable(steppableInstance)

  
CompuCellSetup.mainLoop(sim,simthread,steppableRegistry)
  









  
        
