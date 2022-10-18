Cellular Potts Model was not implemented by our group. We executed simulations using the Compucell3d, available at:

https://compucell3d.org/

Here we present the scripts used to run our cellular Stokes flow simulations on Compucell3d. The version used to 
execute the simulations is the CC3D_3.7.8 for ubuntu linux.

<ol>
  <li> Install Compucell3d </li>
  <li> run tweedit edit files inside project to set model parameters </li>
  <li> Project: Stokes_potts_7.cc3d </li>
  <li> Edit file Stokes_potts_7.py to set: </li>
  <ul>
     <li> Initial number of particles </li>
     <li> tension </li> 
     <li> time steps </li>
     <li> system dimensions </li>
     <li> and others </li>
  </ul>
  <li> Edit Stokes_potts_7Steppables.py to set initial condition parameters </li>
  <ul>
     <li> Alignment </li>
     <li> Activity </li>
     <li> Creation Rate </li>
   </ul>
  <li> In the terminal execute: </li>
  <ul>
    <li> > sh compucell3d.sh </li>  
    <li> > open and run the Stokes_potts_7.cc3d project </li>
  </ul>

</ol> 
