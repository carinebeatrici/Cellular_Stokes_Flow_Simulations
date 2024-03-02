#!/bin/bash

echo "Creating povray files"
gfortran povt.f90 -o pov -O3
./pov

echo "Rendering Images"
for i in *.pov
do
  povray $i +W1334 +H1000 -D 2>/dev/null;
  echo $i
done

echo "Rendering Movie"
for f in ray*.png; do cat "$f"; done | ffmpeg -r 10 -f image2pipe -i - -vcodec libx264 -pix_fmt yuv420p output.avi

