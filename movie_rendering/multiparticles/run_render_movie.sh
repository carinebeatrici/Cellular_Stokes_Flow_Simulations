#!/bin/bash
echo "Creating povray files"
gfortran povt_multi.f90 -o pov -O3
./pov

echo "Rendering Images"
for i in *.pov
do
  povray $1 +W3200 +H2400 -D 2>/dev/null;
  echo $i
done

echo "Rendering Movie"
fmpeg -r 10 -y  -i ray%4d.png  output.avi
