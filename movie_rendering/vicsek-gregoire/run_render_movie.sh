#!/bin/bash
gfortran povt.f90 -o pov -O3
./pov

for i in *.pov
do
  povray $i +W1334 +H1000 -D 2>/dev/null;
  echo $i
done

for f in ray*.png; do cat "$f"; done | ffmpeg -r 10 -f image2pipe -i - -vcodec libx264 -pix_fmt yuv420p output.avi

