Voronoi particles model was not implemented by our group.
We executed simulations using the SAMoS, available at:

https://github.com/sknepneklab/SAMoS.git

Here we present the scripts used to run our
cellular Stokes flow simulations on SAMoS.



To run the voronoi simulation:

<ol>
  <li> Install SAMoS  </li>
  <li> Edit config.conf to set model parameters </li>
  <ul>
     <li> Alignment </li>
     <li> tension </li> 
     <li> time steps </li>
     <li> and others </li>
  </ul>
  <li> Edit make_initial.py to set initial condition parameters </li>
  <ul>
     <li> Initial number of particles </li>
     <li> System dimentions </li>
   </ul>
  <li> In the terminal execute: </li>
  <ul>
    <li> > python nake_initial.py </li>  
    <li> >./samos config.config </li>
  </ul>

</ol> 
